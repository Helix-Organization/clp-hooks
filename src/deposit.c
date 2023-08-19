#include "hookapi.h"
#include "constants.h"
#include "macros.h"
#include "deposit.h"

void handle_deposit(uint8_t* memo, int64_t memo_len) {
    // Get amount and currency of the incoming deposit
    int64_t amount = otxn_amount();
    uint8_t currency[20];
    otxn_field(SBUF(currency), sfAmount);
    // should be tested what will currency be

    // Determine the token id of the incoming deposit
    uint8_t token_id = DETERMINE_CURRENCY(currency);

    // Get price range for the token pair
    uint8_t price_range_id = memo[1];
    
    // Verify price range is valid
    if (price_range_id < 0) {
        rollback(SBUF("Error: Invalid price range provided"), 1);
        return;
    }

    // Add liquidity to the pool at the given price range
    ADD_LIQUIDITY_FOR_RANGE(token_id, price_range_id, amount);

    // Calculate LP tokens to be issued
    int64_t lp_tokens = calculate_lp_tokens(token_id, price_range_id, amount);

    // Issue LP tokens to the user
    emit_lp_tokens(lp_tokens);
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
