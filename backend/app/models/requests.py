from typing import Optional

from pydantic import BaseModel, Field

from app.models.annotations import XrplAddress
from xrpledger.models import Amount


class PaymentRequest(BaseModel):
    """
    A model representing the request for a token swap.
    """

    account: XrplAddress = Field(..., description="The XRPL address of the account initiating the swap")

    destination: XrplAddress = Field(..., description="The XRPL address of the account receiving the swap")

    amount: Amount = Field(..., description="The destination token amount details for the swap")

    send_max: Optional[Amount] = Field(..., description="The source token amount details for the swap")

    deliver_min: Optional[Amount] = Field(..., description="The minimum amount to deliver")

    memo: Optional[str] = Field(..., description="The memo to include in the transaction")

    secret: str
    public: str


class ClpRequest(BaseModel):
    """
    A model representing the request for a transaction to concentrated liquidity pool hooks.
    """

    account: XrplAddress = Field(
        ..., description="The XRPL address of the account initiating the transaction"
    )

    destination: XrplAddress = Field(
        ..., description="The XRPL address of the concentrated liquidity pool account"
    )

    amount: Amount = Field(..., description="The destination token amount details for the transaction")

    price_range: int = Field(..., description="The price range id of the pool.", ge=0, le=255)

    secret: str


class TrustSetRequest(BaseModel):
    """
    A model representing the request for a transaction to set trustline.
    """

    account: XrplAddress = Field(
        ..., description="The XRPL address of the account initiating the transaction"
    )

    amount: Amount = Field(..., description="The destination token amount details for the transaction")

    secret: str
