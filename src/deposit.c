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
    uint8_t currency_id = DETERMINE_CURRENCY(currency);

    // Get price range for the token pair
    uint8_t price_range_id = memo[1];
    
    // Verify price range is valid
    if (price_range_id < 0) {
        rollback(SBUF("Error: Invalid price range provided"), 1);
        return;
    }

    // Add liquidity to the pool at the given price range
    ADD_LIQUIDITY_FOR_RANGE(currency_id, price_range_id, amount);

    // Calculate LP tokens to be issued
    uint64_t lp_token_amount = CALCULATE_LP_TOKEN(currency_id, price_range_id, amount);

    // Issue LP tokens to the user
    EMIT_LP_TOKEN(price_range_id, lp_token_amount);
}

