/**
 * This hook just accepts any transaction coming through it
 */
#include "error.h"
#include "hookapi.h"
#include <stdint.h>

#define DEPOSIT_TRANSACTION 1
#define WITHDRAW_TRANSACTION 2
#define SWAP_TRANSACTION 3
#define EMPTY 4
#define ACCID_SIZE 20

#define GET_TRANSACTION_TYPE_FROM_MEMO(memo)                                   \
  /*                                                                           \
      Args:                                                                    \
          memo: uint8_t*                                                       \
      Return:                                                                  \
          int                                                                  \
          1: deposit                                                           \
          2: withdraw                                                          \
          3: swap                                                              \
  */                                                                           \
  (memo)[5] == 'D'   ? DEPOSIT_TRANSACTION                                     \
  : (memo)[5] == 'W' ? WITHDRAW_TRANSACTION                                    \
  : (memo)[5] == 'S' ? SWAP_TRANSACTION                                        \
  : (memo)[5] == 'E' ? EMPTY                                                   \
                     : 0

#define ADD_LIQUIDITY_FOR_RANGE(currency_id, price_range_id, amount)           \
  /*                                                                           \
      Args:                                                                    \
          currency_id: uint8_t*                                                \
          price_range_id: uint8_t*                                             \
          amount: int64_t                                                      \
      Return:                                                                  \
          void                                                                 \
  */                                                                           \
  uint8_t key[32] = {0};                                                       \
  key[0] = currency_id;                                                        \
  key[1] = price_range_id;                                                     \
  int64_t liquidity;                                                           \
  char liquidity_buf[8];                                                       \
  /* Get current liquidity */                                                  \
  if (state(SBUF(liquidity_buf), SBUF(key)) != sizeof(liquidity)) {            \
    liquidity = 0;                                                             \
  } else {                                                                     \
    liquidity = INT64_FROM_BUF(liquidity_buf);                                 \
  }                                \
  liquidity = 0; \
  /* Add new liquidity and store it to state */                                \
  liquidity += amount;                                                         \
  INT64_TO_BUF(liquidity_buf, liquidity);                                      \
  if (state_set(SBUF(liquidity_buf), SBUF(key)) != sizeof(liquidity)) {        \
    rollback(SBUF("Error: Could not store liquidity state!"), 1);              \
  }

#define CHECK_PRICE_RATE(price_rate, price_range_id)                           \
  /*                                                                           \
      Args:                                                                    \
          memo: uint8_t*                                                       \
          price_range_id: uint8_t                                              \
      Return:                                                                  \
          int64_t                                                              \
          price_rate                                                           \
  */                                                                           \
  price_rate = float_set(-6, price_rate);                                      \
  int64_t price_range = (uint32_t)(price_range_id);                            \
  int64_t tmp_price_rate = price_rate;                                         \
  if (price_range < 127) {                                                     \
    tmp_price_rate = float_invert(tmp_price_rate);                             \
    price_range = 127 - price_range;                                           \
  } else {                                                                     \
    price_range = price_range - 127;                                           \
  }                                                                            \
  int64_t price_rate_err;                                                      \
  if (price_range == 0) {                                                      \
    if (float_compare(price_rate, float_one(), COMPARE_EQUAL) == 0) {          \
      rollback(SBUF("Invalid price_rate"), 1);                                 \
    }                                                                          \
    price_rate_err = float_set(1, 0);                                          \
  } else if (price_range == 1) {                                               \
    price_rate_err = float_sum(                                                \
        float_divide(tmp_price_rate, float_set(-2, 105)), float_set(0, -1));   \
  } else {                                                                     \
    price_rate_err =                                                           \
        float_sum(float_divide(float_root(tmp_price_rate, price_range),        \
                               float_set(-2, 105)),                            \
                  float_set(0, -1));                                           \
  }                                                                            \
  /* price rate has to be within 0.001% error rate */                          \
  if (float_compare(price_rate_err, float_set(-5, -1), COMPARE_LESS) == 1)     \
    rollback(SBUF("Invalid price_rate"), 1);                                   \
  if (float_compare(price_rate_err, float_set(-5, 1), COMPARE_GREATER) == 1)   \
    rollback(SBUF("Invalid price_rate"), 1);

#define UPDATE_LP_TOKENS(price_range_id, input_lp_token_amount)                \
  ({                                                                           \
    uint8_t _key[32] = {0};                                                    \
    _key[0] = 2;                                                               \
    _key[1] = price_range_id;                                                  \
    char _lp_token_amount_buf[8];                                              \
    int64_t _current_lp_token_amount;                                          \
    if (state(SBUF(_lp_token_amount_buf), SBUF(_key)) !=                       \
        sizeof(_current_lp_token_amount))                                      \
      _current_lp_token_amount = 0;                                            \
    else                                                                       \
      _current_lp_token_amount = INT64_FROM_BUF(_lp_token_amount_buf);         \
      _current_lp_token_amount = 0; \
    _current_lp_token_amount += input_lp_token_amount;                         \
    INT64_TO_BUF(_lp_token_amount_buf, _current_lp_token_amount);              \
    if (state_set(SBUF(_lp_token_amount_buf), SBUF(_key)) !=                   \
        sizeof(_current_lp_token_amount)) {                                    \
      rollback(SBUF("Error: Could not store lp token amount state!"), 1);      \
    }                                                                          \
    TRACEVAR(_current_lp_token_amount);\
  })

#define MAKE_AND_EMIT_TX(otxn_accid, currency, issuer_accid, amount)           \
  /*                                                                           \
      Args:                                                                    \
          otxn_accid: uint8_t*                                                 \
          currency: uint8_t*                                                   \
          issuer_accid: uint8_t*                                               \
          amount: int64_t                                                      \
      Return:                                                                  \
          void                                                                 \
  */                                                                           \
  if (float_sto(SBUF(amt_out), SBUF(currency), SBUF(issuer_accid), amount,     \
                amAMOUNT) < 0) {                                               \
    rollback(SBUF("Error: Could not dump LP token amount into sto"),           \
             NOT_AN_AMOUNT);                                                   \
    break;                                                                     \
  }                                                                            \
  PREPARE_PAYMENT_SIMPLE_TRUSTLINE(txn_out, (amt_out_ptr + 1), otxn_accid, 0,  \
                                   0);                                         \
  e = emit(SBUF(emithash), SBUF(txn_out));                                     \
  if (e < 0) {                                                                 \
    rollback(SBUF("Error: Failed to emit LP token!"), e);                      \
    break;                                                                     \
  }

int64_t hook(uint32_t reserved) {

  etxn_reserve(2);
  uint8_t emithash[32];
  uint8_t amt_out[49];
  uint8_t *amt_out_ptr = amt_out;
  uint8_t txn_out[PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE];
  int64_t e;
  int64_t median_rate = float_set(0, 1);

  uint8_t otxn_accid[20];
  int32_t otxn_accid_len = otxn_field(SBUF(otxn_accid), sfAccount);

  // get the account the hook is running on and the account that created the txn
  uint8_t hook_accid[20];
  hook_account(SBUF(hook_accid));

  // compare the "From Account" (sfAccount) on the transaction with the account
  // the hook is running on
  int equal = 0;
  BUFFER_EQUAL(equal, hook_accid, otxn_accid, 20);
  if (equal)
    accept(SBUF("Outgoing transaction"), 20);

  int64_t time = 0;
  uint8_t txq = 0;
  uint8_t currency_in = 0;

  uint8_t lp_currency[ACCID_SIZE] = {0, 0, 0,   0,   0, 0, 0, 0, 0, 0,
                                     0, 0, 'L', 'P', 0, 0, 0, 0, 0, 0};

  uint8_t issuer_accids[2][ACCID_SIZE];
  util_accid(issuer_accids[0], ACCID_SIZE,
             SBUF("rKo8nG3i6qmyhcVYV1njnC5JhEYWnwB51w"));
  util_accid(issuer_accids[1], ACCID_SIZE,
             SBUF("rKo8nG3i6qmyhcVYV1njnC5JhEYWnwB51w"));

  uint8_t c0[ACCID_SIZE] = {0, 0, 0,   0,   0,   0, 0, 0, 0, 0,
                            0, 0, 'X', 'S', 'D', 0, 0, 0, 0, 0};
  uint8_t c1[ACCID_SIZE] = {0, 0, 0,   0,   0,   0, 0, 0, 0, 0,
                            0, 0, 'U', 'S', 'D', 0, 0, 0, 0, 0};

  uint8_t currencies[2][ACCID_SIZE];
  for (int i = 0; GUARD(ACCID_SIZE), i < ACCID_SIZE; i++) {
    currencies[0][i] = c0[i];
    currencies[1][i] = c1[i];
  }

  uint8_t memo[2048];
  int64_t memo_len = otxn_field(SBUF(memo), sfMemos);
  int tx_type = GET_TRANSACTION_TYPE_FROM_MEMO(memo);

  int64_t oslot = otxn_slot(0);
  if (oslot < 0)
    rollback(SBUF("Trade: Could not slot originating txn."), NO_FREE_SLOTS);
  int64_t amt_slot = slot_subfield(oslot, sfAmount, 0);
  if (amt_slot < 0)
    rollback(SBUF("Trade: Could not slot otxn.sfAmount"), NO_FREE_SLOTS);
  int64_t amt = slot_float(amt_slot);
  if (amt < 0)
    rollback(SBUF("Trade: Could not parse amount."), PARSE_ERROR);
  int64_t amount_in = float_int(amt, 6, 0);
  int64_t is_xrp = slot_type(amt_slot, 1);
  if (is_xrp < 0)
    rollback(SBUF("Trade: Could not determine sent amount type"), PARSE_ERROR);
  if (is_xrp == 1)
    rollback(SBUF("XRP is not allowed"), INVALID_ARGUMENT);

  uint8_t amount_buffer[48];
  if (slot(SBUF(amount_buffer), amt_slot) < 48)
    rollback(SBUF("Trade: Could not dump sfAmount"), NOT_AN_AMOUNT);
  uint8_t *currency_ptr = amount_buffer;

  switch (tx_type) {
  case DEPOSIT_TRANSACTION: {
    equal = 0;
    int c;
    for (c = 0; GUARD(2), c < 2 && equal != 1; ++c)
      BUFFER_EQUAL_GUARD(equal, currency_ptr + 8, ACCID_SIZE, currencies[c],
                         ACCID_SIZE, 2);
    if (equal == 1)
      currency_in = c - 1;
    else
      rollback(SBUF("Trade: IOU not supported."), INVALID_ARGUMENT);
    uint8_t price_range_id = memo[14];
    uint8_t currency_id = currency_in;

    ADD_LIQUIDITY_FOR_RANGE(currency_id, price_range_id, amount_in);

    TRACEVAR(liquidity);

    // calculate lp tokens
    // lp tokens is relative to the liquidity provided
    int64_t price_rate =
        (memo)[23] << 24 | (memo)[24] << 16 | (memo)[25] << 8 | (memo)[26];
    CHECK_PRICE_RATE(price_rate, price_range_id);

    int64_t lp_token_amount = float_set(-6, amount_in);
    if (currency_in == 1)
      lp_token_amount = float_multiply(lp_token_amount, price_rate);

    lp_currency[14] = price_range_id;
    MAKE_AND_EMIT_TX(otxn_accid, lp_currency, hook_accid, lp_token_amount);

    UPDATE_LP_TOKENS(price_range_id, float_int(lp_token_amount, 6, 0));
    break;
  }
  case SWAP_TRANSACTION: {
    equal = 0;
    int c;
    for (c = 0; GUARD(2), c < 2 && equal != 1; ++c)
      BUFFER_EQUAL_GUARD(equal, currency_ptr + 8, ACCID_SIZE, currencies[c],
                         ACCID_SIZE, 2);
    if (equal == 1)
      currency_in = c - 1;
    else
      rollback(SBUF("Trade: IOU not supported."), INVALID_ARGUMENT);
    uint8_t price_range_id = memo[14];
    uint8_t currency_id = 1 - currency_in;

    int64_t price_rate =
        (memo[23] << 24) + (memo[24] << 16) + (memo[25] << 8) + memo[26];
    // fee is 0.1%
    amt = float_mulratio(amt, 0, 999000, 1000000);
    price_rate = float_set(-6, price_rate);

    TRACEVAR(float_int(price_rate, 6, 0));

    int64_t return_amount = float_multiply(amt, price_rate);
    if (currency_in == 0) {
      return_amount = float_multiply(amt, price_rate);
    } else {
      return_amount = float_divide(amt, price_rate);
    }

    MAKE_AND_EMIT_TX(otxn_accid, currencies[currency_id],
                     issuer_accids[currency_id], return_amount);

    break;
  }
  case WITHDRAW_TRANSACTION: {
    // check lp token currency
    equal = 0;
    lp_currency[14] = (currency_ptr + 8)[14];
    BUFFER_EQUAL_GUARD(equal, currency_ptr + 8, ACCID_SIZE, lp_currency,
                       ACCID_SIZE, 2);

    if (!equal)
      rollback(SBUF("Invalid LP Token"), 0);

    uint8_t price_range_id = lp_currency[14];

    // calculate token amount
    // token amount is relative portion of lp token

    uint8_t key[32] = {0};
    key[0] = 0;
    key[1] = price_range_id;

    int64_t liquidity[2];
    char liquidity_buf[8];
    /* Get current liquidity */
    if (state(SBUF(liquidity_buf), SBUF(key)) != 8) {
      rollback(SBUF("liqudity pool not found"), 0);
    } else {
      liquidity[0] = INT64_FROM_BUF(liquidity_buf);
    }

    TRACEVAR(liquidity[0]);

    /* Get current liquidity */
    key[0] = 1;
    if (state(SBUF(liquidity_buf), SBUF(key)) != 8)
      rollback(SBUF("liqudity pool not found"), 0);

    liquidity[1] = INT64_FROM_BUF(liquidity_buf);

    key[0] = 2;
    char total_lp_amount_buf[8];
    int64_t total_lp_amount;
    if (state(SBUF(total_lp_amount_buf), SBUF(key)) != 8)
      rollback(SBUF("lp not found"), 0);
    total_lp_amount = INT64_FROM_BUF(total_lp_amount_buf);

    TRACEVAR(total_lp_amount);

    // calculate lp token portion
    int64_t portion =
        float_divide(float_set(0, amount_in), float_set(0, total_lp_amount));

    // calculate return token amount
    int64_t return_token_amount[2];
    return_token_amount[0] =
        float_multiply(float_set(-6, liquidity[0]), portion);
    return_token_amount[1] =
        float_multiply(float_set(-6, liquidity[1]), portion);

    TRACEVAR(float_int(portion, 6, 0));
    TRACEVAR(float_int(return_token_amount[1], 6, 0))

    // emit txs for returning tokens
    MAKE_AND_EMIT_TX(otxn_accid, currencies[0], issuer_accids[0],
                     return_token_amount[0]);
    MAKE_AND_EMIT_TX(otxn_accid, currencies[1], issuer_accids[1],
                     return_token_amount[1]);

    // remove from liquidity
    key[0] = 0;
    key[2] = 0;
    liquidity[0] -= float_int(return_token_amount[0], 6, 0);
    INT64_TO_BUF(liquidity_buf, liquidity[0]);
    if (state_set(SBUF(liquidity_buf), SBUF(key)) != 8) {
      rollback(SBUF("Error: Could not store liquidity state!"), 1);
    }
    key[0] = 1;
    liquidity[1] -= float_int(return_token_amount[1], 6, 0);
    INT64_TO_BUF(liquidity_buf, liquidity[1]);
    if (state_set(SBUF(liquidity_buf), SBUF(key)) != 8) {
      rollback(SBUF("Error: Could not store liquidity state!"), 1);
    }

    // remove from total lp
    total_lp_amount -= amount_in;
    INT64_TO_BUF(total_lp_amount_buf, total_lp_amount);
    if (state_set(SBUF(total_lp_amount_buf), SBUF(key)) != 8) {
      rollback(SBUF("Error: Could not store total lp token amount state!"), 1);
    }
  } break;
  }
  accept(0, 0, 0);
  return 0;
}