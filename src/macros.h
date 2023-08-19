// Utility macros for the hook

#define DEPOSIT_TRANSACTION    1
#define WITHDRAW_TRANSACTION   2
#define SWAP_TRANSACTION       3

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
    (memo)[0] == 'D' ? DEPOSIT_TRANSACTION :  \
    (memo)[0] == 'W' ? WITHDRAW_TRANSACTION : \
    (memo)[0] == 'S' ? SWAP_TRANSACTION :     \
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
    token == TOKEN_A ? 1 : \
    token == TOKEN_B ? 2 : \
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
        Return:
            void
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

#define REMOVE_LIQUIDITY_FOR_RANGE(currency_id, price_range_id, amount) \
    /* \
        Args:                            \
            currency_id: uint8_t         \
            price_range_id: uint8_t      \
            amount: int64_t              \
        Return:
            void
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
        if (liquidity < amount) { \
            rollback(SBUF("Error: Not enough liquidity!"), 1); \
        } \
        liquidity -= amount; \
        if (state_set(SBUF(liquidity), SBUF(key)) != sizeof(liquidity)) { \
            rollback(SBUF("Error: Could not store liquidity state!"), 1); \
        } \
    } while(0)
