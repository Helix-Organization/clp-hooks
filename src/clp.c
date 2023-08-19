#include "hookapi.h"
#include "constants.h"
#include "macros.h"
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
        rollback(SBUF("Error: Only payment transactions are allowed"), 1);
        return 1;
    }

    // Fetch the memo field to determine the transaction type
    uint8_t memo[2048];
    int64_t memo_len = otxn_field(memo, sizeof(memo), sfMemos);

    // Assuming the transaction type (deposit, withdraw, swap) is encoded in the memo field.
    // Adjust as per actual implementation.
    int tx_type = GET_TRANSACTION_TYPE_FROM_MEMO(memo);

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
            rollback(SBUF("Error: Invalid transaction type"), 1);
            break;
    }

    return 0;
}
