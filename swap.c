#include "extern.h"
#include "macro.h"
#include "sfcodes.h"
#include "constants.h"
#include "swap.h"

void handle_swap(uint8_t* memo, int64_t memo_len) {
    uint8_t tokenBuffer[20];
    // 1. Check which asset is being sent.
    if (otxn_field(SBUF(tokenBuffer), sfAmount) != "LPToken") {
        rollback(SBUF("Failed to get transaction send amount."), 1);
        return;
    }
    
    uint64_t assetSent = parse_amount_from_buffer(tokenBuffer);
    if (assetSent == 0) {
        rollback(SBUF("Invalid asset amount."), 1);
        return;
    }

    bool isAssetA = check_if_assetA(tokenBuffer);

    // 2. Calculate the amount of the other asset to return.
    uint64_t assetToReturn;
    if (isAssetA) {
        assetToReturn = calculate_asset_to_return(assetSent, totalAssetAInPool, totalAssetBInPool);
    } else {
        assetToReturn = calculate_asset_to_return(assetSent, totalAssetBInPool, totalAssetAInPool);
    }

    // 3. Adjust the pool's reserves accordingly.
    if (isAssetA) {
        adjust_pool_reserves(assetSent, -assetToReturn);
    } else {
        adjust_pool_reserves(-assetToReturn, assetSent);
    }

    // 4. Send the calculated amount of the other asset to the user.
    emit_asset_return(assetToReturn, isAssetA ? TOKEN_B : TOKEN_A);
}

uint64_t calculate_asset_to_return(uint64_t assetSent, uint64_t reserveSentAsset, uint64_t reserveReturnAsset) {
    // Using the Constant Product formula
    uint64_t k = reserveSentAsset * reserveReturnAsset;
    uint64_t newReserveSentAsset = reserveSentAsset + assetSent;
    uint64_t newReserveReturnAsset = k / newReserveSentAsset;
    
    return reserveReturnAsset - newReserveReturnAsset;
}
