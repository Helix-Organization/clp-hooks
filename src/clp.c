#include "hookapi.h"

/**/
#define DEPOSIT_TRANSACTION    1
#define WITHDRAW_TRANSACTION   2
#define SWAP_TRANSACTION       3

#define GET_TRANSACTION_TYPE_FROM_MEMO(memo_0)   \
    /* \
        Args:
            memo_0: uint8_t
        Return:
            int
            1: deposit
            2: withdraw
            3: swap
    */ \
    (memo_0) == 'D' ? DEPOSIT_TRANSACTION :  \
    (memo_0) == 'W' ? WITHDRAW_TRANSACTION : \
    (memo_0) == 'S' ? SWAP_TRANSACTION :     \
    0


#define TOKEN_A "TokenA.A's_issuer_address"
#define TOKEN_B "TokenB.B's_issuer_address"

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


// Macros for Liquidity Pool

#define TOKEN_A_KEY 0x01U
#define TOKEN_B_KEY 0x02U

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
        key[0] = (currency_id); \
        key[1] = (price_range_id); \
        uint64_t liquidity; \
        /* Get current liquidity */ \
        if (state(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not read liquidity state"), 1); \
        } \
        /* Add new liquidity and store it to state */ \
        liquidity += (amount); \
        if (state_set(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not store liquidity state!"), 1); \
        } \
    } while(0)

#define REMOVE_LIQUIDITY_FOR_RANGE(currency_id, price_range_id, amount) \
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
        key[0] = (currency_id); \
        key[1] = (price_range_id); \
        uint64_t liquidity; \
        /* Get current liquidity */ \
        if (state(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not read liquidity state"), 1); \
        } \
        /* Add new liquidity and store it to state */ \
        if (liquidity < (amount)) { \
            rollback(SBUF("Error: Not enough liquidity!"), 1); \
        } \
        liquidity -= (amount); \
        if (state_set(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not store liquidity state!"), 1); \
        } \
    } while(0)

// LP Token Macros
#define CALCULATE_LP_TOKEN(currency_id, price_range_id, amount) \
    /* \
        Description:                                          \
            Calculates the amount of LP tokens to be emitted. \
        Args:                                                 \
            currency_id: uint8_t                              \
            price_range_id: uint8_t                           \
            amount: int64_t                                   \
        Return:                                               \
            int64_t                                           \
    */ \
    ({ \
        uint8_t key[32] = {0}; \
        key[0] = (currency_id); \
        key[1] = (price_range_id); \
        uint64_t total_liquidity; \
        /* Get current liquidity */ \
        if (state(SBUF(total_liquidity), SBUF(key)) != sizeof(total_liquidity)) { \
            rollback(SBUF("Error: Could not read liquidity state"), 1); \
        } \
        /* Calculate LP tokens */ \
        ((amount) * 1000) / total_liquidity; \
    })

#define LP_TOKEN_CURRENCY "LP_token_symbol+price_range_id"
#define LP_TOKEN_ISSUER "LP_token_issuer_address"

#define EMIT_LP_TOKEN(price_range_id, lp_token_amount) \
    /* \
        Description:                                                                          \
            Emits LP tokens to the sender of the deposit transaction.                         \
        Args:                                                                                 \
            price_range_id: uint8_t (Price range ID representing the specific liquidity pool) \
            lp_token_amount: int64_t (Amount of LP tokens to be emitted)                      \
        Return:                                                                               \
            void                                                                              \
    */ \
    do { \
        uint8_t emithash[32]; \
        uint8_t amt_out[49]; \
        uint8_t *amt_out_ptr = amt_out; \
        if (float_sto(SBUF(amt_out), SBUF(LP_TOKEN_CURRENCY), SBUF(LP_TOKEN_ISSUER), float_set(-6, (lp_token_amount)), amAMOUNT) < 0) { \
            rollback(SBUF("Error: Could not dump LP token amount into sto"), NOT_AN_AMOUNT); \
            break; \
        } \
        uint8_t tx[PREPARE_PAYMENT_SIMPLE_TRUSTLINE_SIZE]; \
        PREPARE_PAYMENT_SIMPLE_TRUSTLINE(tx, (amt_out_ptr + 1), otxn_sender(), 20 + (price_range_id), 0); \
        int64_t e = emit(SBUF(emithash), SBUF(tx)); \
        if (e < 0) { \
            rollback(SBUF("Error: Failed to emit LP token!"), e); \
            break; \
        } \
    } while(0)


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

#define HANDLE_WITHDRAW() 2

#define HANDLE_SWAP() 3

/* Main Function */

int64_t hook(int32_t reserved) {
    // Check if the transaction is a payment transaction
    if (otxn_type() != ttPAYMENT) {
        rollback(SBUF("Error: Only payment transactions are allowed"), 1);
        return 1;
    }

    // Fetch the memo field to determine the transaction type
    uint8_t memo[2048];
    int64_t memo_len = otxn_field(memo, sizeof(memo), sfMemos);

    // Assuming the transaction type (deposit, withdraw, swap) is encoded in the memo field.
    // Adjust as per actual implementation.
    uint8_t tx_type = GET_TRANSACTION_TYPE_FROM_MEMO(memo[0]);

    TRACESTR("Accept.c: Called.");

    switch (tx_type) {
        case DEPOSIT_TRANSACTION:
            HANDLE_DEPOSIT(memo, memo_len);
            break;
        case WITHDRAW_TRANSACTION:
            HANDLE_WITHDRAW();
            break;
        case SWAP_TRANSACTION:
            HANDLE_SWAP();
            break;
        default:
            rollback(SBUF("Error: Invalid transaction type"), 1);
            break;
    }

    accept(SBUF("Done"), 0);
    _g(1,1);
    return 0;
}