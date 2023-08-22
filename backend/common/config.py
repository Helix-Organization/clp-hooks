import os

from dotenv import load_dotenv
from pydantic_settings import BaseSettings
from xrpl.wallet import Wallet


class CommonSettings(BaseSettings):
    """
    Common settings for the API.
    """

    # Project
    PROJECT_NAME: str = "XRPL Swap API"
    API_VERSION: str = "0.0.1"
    API_PREFIX: str = "/api/v1"

    # FastAPI
    LIVE_RELOAD: bool = False

    # Redis
    REDIS_HOST: str = "localhost"
    REDIS_PORT: int = 6379
    REDIS_DB: int = 0

    # XRP Ledger
    json_rpc_url: str
    websocket_url: str
    wallet: Wallet

    class Config:
        """
        Configuration for the settings.
        """

        env_file = ".env"
        case_sensitive = True
        extra = "ignore"


class DevSettings(CommonSettings):
    """
    Development settings for the API.
    """

    # FastAPI
    LIVE_RELOAD: bool = True

    # Redis
    REDIS_HOST: str = "localhost"
    REDIS_PORT: int = 6379
    REDIS_DB: int = 0

    # XRP Ledger
    json_rpc_url: str = "https://s.altnet.rippletest.net:51234"  # Testnet
    websocket_url: str = "wss://s.altnet.rippletest.net:51233"  # Testnet
    wallet: Wallet = Wallet.from_secret("sszAbUiqRJsqjQbH4pJmtxrBd3Lfw")


class AmmDevSettings(CommonSettings):
    """
    AMM-Devnet settings for the API.
    """

    # FastAPI
    LIVE_RELOAD: bool = True

    # Redis
    REDIS_HOST: str = "localhost"
    REDIS_PORT: int = 6379
    REDIS_DB: int = 0

    # XRP Ledger
    json_rpc_url: str = "https://amm.devnet.rippletest.net:51234"  # AMM-Devnet
    websocket_url: str = "wss://amm.devnet.rippletest.net:51233"
    wallet: Wallet = Wallet.from_secret("sszAbUiqRJsqjQbH4pJmtxrBd3Lfw")


class HooksDevSettings(CommonSettings):
    """_summary_
    Hooks-Devnet settings for the API.
    """

    # FastAPI
    LIVE_RELOAD: bool = True

    # Redis
    REDIS_HOST: str = "localhost"
    REDIS_PORT: int = 6379
    REDIS_DB: int = 0

    # XRP Ledger
    json_rpc_url: str = "https://hooks-testnet-v3.xrpl-labs.com"  # AMM-Devnet
    websocket_url: str = "wss://hooks-testnet-v3.xrpl-labs.com"
    wallet: Wallet = Wallet.from_secret("sszAbUiqRJsqjQbH4pJmtxrBd3Lfw")


class ProdSettings(CommonSettings):
    """
    Production settings for the API.
    """

    # FastAPI
    LIVE_RELOAD: bool = False

    # Redis
    REDIS_HOST: str = "localhost"
    REDIS_PORT: int = 6379
    REDIS_DB: int = 0

    # XRP Ledger
    json_rpc_url: str = "https://s1.ripple.com:51234"  # Mainnet
    websocket_url: str = "wss://xrpl.ws/"
    wallet: Wallet = Wallet.from_secret("sszAbUiqRJsqjQbH4pJmtxrBd3Lfw")


def get_settings() -> CommonSettings:
    """
    Get the settings for the API.

    Returns:
        CommonSettings: The settings for the API.
    """
    settings_map = {
        "prod": ProdSettings(),
        "dev": DevSettings(),
        "amm_dev": AmmDevSettings(),
        "hooks_dev": HooksDevSettings(),
    }

    load_dotenv()
    return settings_map.get(os.getenv("ENV", "dev"), DevSettings())


settings = get_settings()
