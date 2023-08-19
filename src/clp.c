#include "hookapi.h"
#include "sfcodes.h"
#include "error.h"
#include "constants.h"
#include "deposit.h"
#include "withdraw.h"
#include "swap.h"

// Main entry point
int64_t cbak(int32_t reserved) {
    return 0;
}

int64_t hook(int32_t reserved) {
    // Check if the transaction is a payment transaction
    if (otxn_type() != ttPAYMENT) {
        rollback("Only payment transactions are allowed", 1);
        return 1;
    }

    // Fetch the memo field to determine the transaction type
    uint8_t memo[2048];
    int64_t memo_len = otxn_field(memo, sizeof(memo), sfMemos);

    // Assuming the transaction type (deposit, withdraw, swap) is encoded in the memo field.
    // Adjust as per actual implementation.
    int tx_type = get_transaction_type_from_memo(memo, memo_len);

    switch (tx_type) {
        case DEPOSIT_TRANSACTION:
            handle_deposit(memo, memo_len);
            break;
        case WITHDRAW_TRANSACTION:
            handle_withdraw(memo, memo_len);
            break;
        case SWAP_TRANSACTION:
            handle_swap(memo, memo_len);
            break;
        default:
            rollback("Invalid transaction type", 1);
            break;
    }

    return 0;
}

int get_transaction_type_from_memo(uint8_t* memo, int64_t memo_len) {
    // Extract transaction type from memo
    // Placeholder logic, adjust as needed
    if (memo[0] == 'D') return DEPOSIT_TRANSACTION;
    if (memo[0] == 'W') return WITHDRAW_TRANSACTION;
    if (memo[0] == 'S') return SWAP_TRANSACTION;
    return 0;
}





