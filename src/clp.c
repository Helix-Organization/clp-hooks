/**
 * This hook just accepts any transaction coming through it
 */
#include "error.h"
#include "hookapi.h"
#include <stdint.h>

#define DEPOSIT_TRANSACTION    1
#define WITHDRAW_TRANSACTION   2
#define SWAP_TRANSACTION       3
#define ACCID_SIZE             20

#define GET_TRANSACTION_TYPE_FROM_MEMO(memo)   \
    /* \
        Args:                   \
            memo: uint8_t*      \
        Return:                 \
            int                 \
            1: deposit          \
            2: withdraw         \
            3: swap             \
    */ \
    (memo)[5] == 'D' ? DEPOSIT_TRANSACTION :  \
    (memo)[5] == 'W' ? WITHDRAW_TRANSACTION : \
    (memo)[5] == 'S' ? SWAP_TRANSACTION :     \
    0

#define ADD_LIQUIDITY_FOR_RANGE(currency_id, price_range_id, amount) \
    /* \
        Args:                            \
            currency_id: uint8_t         \
            price_range_id: uint8_t      \
            amount: int64_t              \
        Return:                          \
            void                         \
    */ \
    do { \
        uint8_t key[32] = {0}; \
        key[0] = currency_id; \
        key[1] = price_range_id; \
        uint64_t liquidity; \
        /* Get current liquidity */ \
        if (state(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not read liquidity state"), 1); \
        } \
        /* Add new liquidity and store it to state */ \
        liquidity += amount; \
        if (state_set(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not store liquidity state!"), 1); \
        } \
    } while(0)

#define DETERMINE_CURRENCY(token) \
    /* \
        Args:                       \
            currency: uint8_t*      \
        Return:                     \
            int                     \
            1: token A              \
            2: token B              \
            0: error                \
    */ \
    (token) == TOKEN_A ? 1 : \
    (token) == TOKEN_B ? 2 : \
    0

#define HANDLE_DEPOSIT(memo, memo_len)               \
    do {                                            \
        int64_t _amount = otxn_amount();            \
        uint8_t _currency[20];                      \
        otxn_field(SBUF(_currency), sfAmount);      \
                                                    \
        uint8_t _currency_id = DETERMINE_CURRENCY(_currency); \
                                                    \
        uint8_t _price_range_id = (memo)[1];        \
        if (_price_range_id < 0) {                  \
            rollback(SBUF("Error: Invalid price range provided"), 1); \
            return;                                \
        }                                           \
                                                    \
        ADD_LIQUIDITY_FOR_RANGE(_currency_id, _price_range_id, _amount); \
                                                    \
        uint64_t _lp_token_amount = CALCULATE_LP_TOKEN(_currency_id, _price_range_id, _amount); \
                                                    \
        EMIT_LP_TOKEN(_price_range_id, _lp_token_amount); \
    } while(0)



int64_t hook(uint32_t reserved ) {

    uint8_t otxn_accid[20];
    int32_t otxn_accid_len = otxn_field(SBUF(otxn_accid), sfAccount);

    // get the account the hook is running on and the account that created the txn
    uint8_t hook_accid[20];
    hook_account(SBUF(hook_accid));

    // compare the "From Account" (sfAccount) on the transaction with the account the hook is running on
    int equal = 0; BUFFER_EQUAL(equal, hook_accid, otxn_accid, 20);
    if (equal)
        accept(SBUF("Peggy: Outgoing transaction"), 20);

    int64_t time = 0;
    uint8_t txq = 0;
    uint8_t currency_in = 0;

    uint8_t c0[ACCID_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'X', 'S', 'D', 0, 0, 0, 0, 0};
    uint8_t c1[ACCID_SIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'U', 'S', 'D', 0, 0, 0, 0, 0};
    uint8_t currencies[2][ACCID_SIZE];
    for (int i = 0; GUARD(ACCID_SIZE), i < ACCID_SIZE; i++)
    {
        currencies[0][i] = c0[i];
        currencies[1][i] = c1[i];
    }

    uint8_t memo[2048];
    int64_t memo_len = otxn_field(SBUF(memo), sfMemos);
    int tx_type = GET_TRANSACTION_TYPE_FROM_MEMO(memo);

    switch(tx_type) {
        case DEPOSIT_TRANSACTION: {
            TRACESTR("DEPOSIT");
            // Originating tx
            int64_t oslot = otxn_slot(0);
            if (oslot < 0)
                rollback(SBUF("Trade: Could not slot originating txn."), NO_FREE_SLOTS);
            int64_t amt_slot = slot_subfield(oslot, sfAmount, 0);
            if (amt_slot < 0)
                rollback(SBUF("Trade: Could not slot otxn.sfAmount"), NO_FREE_SLOTS);
            int64_t amt = slot_float(amt_slot);
            if (amt < 0)
                rollback(SBUF("Trade: Could not parse amount."), PARSE_ERROR);
            uint64_t amount_in = float_int(amt, 6, 0);
            int64_t is_xrp = slot_type(amt_slot, 1);
            if (is_xrp < 0)
                rollback(SBUF("Trade: Could not determine sent amount type"), PARSE_ERROR);
            if (is_xrp != 1)
            {
                uint8_t amount_buffer[48];
                if (slot(SBUF(amount_buffer), amt_slot) < 48)
                    rollback(SBUF("Trade: Could not dump sfAmount"), NOT_AN_AMOUNT);
                uint8_t *currency_ptr = amount_buffer;
                TRACEHEX(amount_buffer);
                equal = 0;
                int c;
                for (c = 0; GUARD(2), c < 2 && equal != 1; ++c)
                    BUFFER_EQUAL_GUARD(equal, currency_ptr + 8, ACCID_SIZE, currencies[c], ACCID_SIZE, 2);
                if (equal == 1)
                    currency_in = c - 1;
                else
                    rollback(SBUF("Trade: IOU not supported."), INVALID_ARGUMENT);
            } 
            uint8_t price_range_id = memo[14];
            uint8_t currency_id = currency_in;
            
            uint8_t key[32] = {0}; 
            key[0] = currency_id; 
            key[1] = price_range_id;
            TRACEVAR(currency_id);
            TRACEVAR(price_range_id);

            uint64_t liquidity; 
            char liquidity_buf[8];
            /* Get current liquidity */ 
            if (state(SBUF(liquidity_buf), SBUF(key)) != sizeof(liquidity)) { 
                liquidity = 0;
            }
            else {
                liquidity = UINT64_FROM_BUF(liquidity_buf);
            }
            /* Add new liquidity and store it to state */ 
            liquidity += amount_in;
            UINT64_TO_BUF(liquidity_buf, liquidity);
            if (state_set(SBUF(liquidity_buf), SBUF(key)) != sizeof(liquidity)) { 
                rollback(SBUF("Error: Could not store liquidity state!"), 1);
            } 

            // calculate lp tokens
            // lp tokens is relative to the liquidity provided
            int64_t lp_token_amount = (int64_t) amount_in;
            TRACEVAR(lp_token_amount);

            uint8_t emithash[32];
            uint8_t amt_out[49];
            uint8_t *amt_out_ptr = amt_out;
            // 7a3919b52247c49bbad9f97737a1db6513a6f70f37603af84f
            uint8_t lp_token_issuer[20] = { 0x7a, 0x39, 0x19, 0xb5, 0x22, 0x47, 0xc4, 0x9b, 0xba, 0xd9, 0xf9, 0x77, 0x37, 0xa1, 0xdb, 0x65, 0x13, 0xa6, 0xf7, 0x0f };
            if (float_sto(SBUF(amt_out), SBUF(currencies[1]), SBUF(lp_token_issuer), float_set(-6, 1), amAMOUNT) < 0) {
                rollback(SBUF("Error: Could not dump LP token amount into sto"), NOT_AN_AMOUNT); \
                break;
            }
            uint8_t txn_out[PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE];

            etxn_reserve(1);
            PREPARE_PAYMENT_SIMPLE_TRUSTLINE(txn_out, amt_out, otxn_accid, 0, 0);
            int64_t e = emit(SBUF(emithash), SBUF(txn_out));
            if (e < 0) {
                rollback(SBUF("Error: Failed to emit LP token!"), e);
                break;
            }
        }

    }
    accept(0, 0, 0);
    return 0;
}