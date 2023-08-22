import { Modal } from "antd";
import * as S from "./SelectTokenModal.styled";
import { SelectTokenModalProps } from "./SelectTokenModal.types";

const TEMP_TOKEN_LIST = [
  {
    name: "ETH",
    logoUrl:
      "https://assets.coingecko.com/coins/images/279/small/ethereum.png?1595348880",
  },
  {
    name: "BTC",
    logoUrl:
      "https://assets.coingecko.com/coins/images/1/small/bitcoin.png?1547033579",
  },
  {
    name: "ADA",
    logoUrl:
      "https://assets.coingecko.com/coins/images/975/small/cardano.png?1547034860",
  },
  {
    name: "BNB",
    logoUrl:
      "https://assets.coingecko.com/coins/images/825/small/binance-coin-logo.png?1547034615",
  },
  {
    name: "USDT",
    logoUrl:
      "https://assets.coingecko.com/coins/images/325/small/Tether-logo.png?1598003707",
  },
  {
    name: "XRP",
    logoUrl:
      "https://assets.coingecko.com/coins/images/44/small/xrp-symbol-white-128.png?1605778731",
  },
  {
    name: "DOGE",
    logoUrl:
      "https://assets.coingecko.com/coins/images/5/small/dogecoin.png?1547792256",
  },
];

const SelectTokenModal = ({
  tokens,
  onClose,
  onSelect,
}: SelectTokenModalProps) => {
  return (
    <Modal
      width={420}
      open={true}
      title="Select a token"
      onCancel={onClose}
      footer={null}
    >
      <S.TokenList>
        {TEMP_TOKEN_LIST.map((token) => (
          <S.TokenItem
            key={token.name}
            onClick={() => {
              onSelect(token.name);
              onClose();
            }}
          >
            <img src={token.logoUrl} alt={token.name} />
            {token.name}
          </S.TokenItem>
        ))}
      </S.TokenList>
    </Modal>
  );
};

export default SelectTokenModal;
