#include "extern.h"
#include "macro.h"
#include "sfcodes.h"
#include "constants.h"
#include "deposit.h"

void handle_deposit(uint8_t* memo, int64_t memo_len) {
    // Get amount and currency of the incoming deposit
    int64_t amount = otxn_amount();
    uint8_t currency[20];
    otxn_field(SBUF(currency), sfAmount);
    // should be tested what will currency be

    // Check if the deposited currency is one of the accepted token pairs
    if (!is_accepted_currency(currency)) {
        rollback(SBUF("Unsupported currency for deposit"), 1);
        return;
    }

    // Get price range for the token pair
    int64_t price_range = determine_price_range(memo, memo_len);
    
    // Verify price range is valid
    if (price_range < 0) {
        rollback(SBUF("Invalid price range provided"), 1);
        return;
    }

    // Add liquidity to the pool at the given price range
    add_liquidity_to_range(currency, amount, price_range);

    // Calculate LP tokens to be issued
    int64_t lp_tokens = calculate_lp_tokens(currency, amount, price_range);

    // Issue LP tokens to the user
    emit_lp_tokens(lp_tokens);
}

int is_accepted_currency(uint8_t* currency) {
    if (strcmp(currency, TOKEN_A) == 0 || strcmp(currency, TOKEN_B) == 0) {
        return 1;
    }
    return 0;
}

int64_t determine_price_range(uint8_t* memo, int64_t memo_len) {
    // Placeholder logic to get the price range from the memo
    // Adjust based on how you encode the price range in the memo
    if (memo[1] == '1') return 10; // price range 10~11
    if (memo[1] == '2') return 11; // price range 11~12
    // ... add more price ranges as needed
    return -1; // invalid price range
}

void add_liquidity_to_range(uint8_t* currency, int64_t amount, int64_t price_range) {
    // Logic to add liquidity to the pool at the given price range
    // This will involve updating the state of the pool for the price range
    // and adjusting the total liquidity for that range.
    // ...
}

int64_t calculate_lp_tokens(uint8_t* currency, int64_t amount, int64_t price_range) {
    // Logic to calculate the number of LP tokens to be issued based on the
    // amount of liquidity provided and the current total liquidity in the price range.
    // This might involve a pro-rata calculation.
    // Placeholder logic:
    int64_t total_liquidity_in_range = get_total_liquidity_in_range(price_range);
    return (amount * 1000) / total_liquidity_in_range; // adjust formula as needed
}

void emit_lp_tokens(int64_t lp_tokens) {
    // Logic to emit LP tokens to the user
    // This can be done using the `emit` function as shown in the examples
    // ...
}
