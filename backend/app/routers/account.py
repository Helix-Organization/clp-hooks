from fastapi import APIRouter, Depends
from fastapi.responses import JSONResponse
from xrpl.models.transactions import TrustSet
from xrpl.wallet import Wallet

from app.models import AccountInfoResponse, TrustSetRequest, XrplAddress, XrplClient
from xrpledger.client import get_xrpl_client
from xrpledger.request import fetch_account_info
from xrpledger.transaction import submit_transaction

router = APIRouter(
    prefix="/account",
    tags=["account"],
    dependencies=[Depends(get_xrpl_client)],
    responses={"400": {"description": "Request failed"}},
)


@router.get("/{account}/info", response_model=AccountInfoResponse)
async def get_account_info(
    account: XrplAddress,
    client: XrplClient,
) -> JSONResponse:
    """
    Fetches the account information for a given XRPL address.
    """
    result = await fetch_account_info(address=account, client=client)

    status_code = 400 if "error" in result else 200

    return JSONResponse(content=result, status_code=status_code)


@router.post("/trustline")
async def set_trustline(request: TrustSetRequest, client: XrplClient) -> JSONResponse:
    """
    Set trustline
    """
    transaction = TrustSet(
        account=request.account,
        limit_amount=request.amount.to_xrpl_amount(),  # type: ignore
    )

    result = await submit_transaction(
        transaction=transaction,
        wallet=Wallet.from_secret(request.secret),
        client=client,
    )

    status_code = 400 if "error" in result else 200

    return JSONResponse(content=result, status_code=status_code)
