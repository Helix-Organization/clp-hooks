// Utility macros for the hook

#define DEPOSIT_TRANSACTION    1
#define WITHDRAW_TRANSACTION   2
#define SWAP_TRANSACTION       3

#define GET_TRANSACTION_TYPE_FROM_MEMO(memo)   \
    ((memo)[0] == 'D' ? DEPOSIT_TRANSACTION :  \
     (memo)[0] == 'W' ? WITHDRAW_TRANSACTION : \
     (memo)[0] == 'S' ? SWAP_TRANSACTION :     \
     0)


