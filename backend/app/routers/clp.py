from fastapi import APIRouter, Depends, HTTPException
from fastapi.responses import JSONResponse
from xrpl.models.transactions import Memo, Payment
from xrpl.wallet import Wallet

from app.models import ClpRequest, XrplClient
from xrpledger.client import get_xrpl_client
from xrpledger.transaction import submit_transaction

router = APIRouter(
    prefix="/clp",
    tags=["concentrated liquidity pool"],
    dependencies=[Depends(get_xrpl_client)],
)

pool_accounts = {
    "rssM7AC37o7Qw8DkuWYeqnnSDJvaC6CJDP": {1: "USD", 2: "BTC"},
}


def str_to_hex(input_str: str) -> str:
    """Converts a string to its hexadecimal representation."""
    return input_str.encode().hex()


def int_to_hex(input_int: int, length: int) -> str:
    """Converts an integer (0-255) to a two-digit hexadecimal representation."""
    max_num = 16**length - 1
    if 0 <= input_int <= max_num:
        return format(input_int, f"0{length}x")
    raise ValueError(f"Integer should be between 0 and {max_num}.")


@router.post("/deposit")
async def deposit_clp(request: ClpRequest, client: XrplClient) -> JSONResponse:
    """
    Send Deposit transaction to a concentrated liquidity pool.
    """
    if request.destination not in pool_accounts:
        raise HTTPException(status_code=400, detail="Invalid pool address")

    if request.amount.symbol not in pool_accounts[request.destination].values():
        raise HTTPException(status_code=400, detail="Invalid token")

    transaction_type_memo = Memo(memo_data=str_to_hex("D"))
    price_range_memo = Memo(memo_data=int_to_hex(request.price_range, 2))
    scale_memo = Memo(memo_data=int_to_hex(int(1.05 ** (request.price_range - 127) * 1_000_000), 8))

    transaction = Payment(
        account=request.account,
        destination=request.destination,
        amount=request.amount.to_xrpl_amount(),
        memos=[transaction_type_memo, price_range_memo, scale_memo],
        flags=2147483648,
        fee="20000",
    )

    result = await submit_transaction(
        transaction=transaction,
        wallet=Wallet.from_secret(request.secret),
        client=client,
    )

    status_code = 400 if "error" in result else 200

    return JSONResponse(content=result, status_code=status_code)
