#include "extern.h"
#include "macro.h"
#include "sfcodes.h"
#include "constants.h"
#include "withdraw.h"

void handle_withdraw(uint8_t* memo, int64_t memo_len)) {
    uint8_t tokenBuffer[20];
    // 1. Check if the incoming transaction contains LP tokens from our pool.
    if (otxn_field(SBUF(tokenBuffer), sfAmount) != "LPToken") {
        rollback(SBUF("Failed to get transaction send amount."), 1);
        return;
    }
    
    uint64_t lpTokensToWithdraw = parse_amount_from_buffer(tokenBuffer);
    if (lpTokensToWithdraw == 0) {
        rollback(SBUF("Invalid LP token amount."), 1);
        return;
    }

    // 2. Calculate the amount of each asset to return based on the number of LP tokens.
    uint64_t assetAtoReturn, assetBtoReturn;
    calculate_assets_to_return(lpTokensToWithdraw, &assetAtoReturn, &assetBtoReturn);

    // 3. Adjust the pool's reserves and total LP tokens accordingly.
    adjust_pool_reserves(-assetAtoReturn, -assetBtoReturn);
    adjust_total_LP_tokens(-lpTokensToWithdraw);

    // 4. Send the appropriate amount of each asset back to the user.
    emit_asset_return(assetAtoReturn, TOKEN_A);
    emit_asset_return(assetBtoReturn, TOKEN_B);
}

void calculate_assets_to_return(uint64_t lpTokens, uint64_t* assetA, uint64_t* assetB) {
    // This is a simple proportional calculation. In a real-world scenario, 
    // you would need to account for fees, slippage, etc.
    *assetA = (lpTokens * totalAssetAInPool) / totalLPTokens;
    *assetB = (lpTokens * totalAssetBInPool) / totalLPTokens;
}
