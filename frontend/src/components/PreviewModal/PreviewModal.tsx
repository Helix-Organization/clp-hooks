import { Button, Modal } from "antd";
import * as S from "./PreviewModal.styled";
import { PreviewModalProps } from "./PreviewModal.types";
import { useParams, useSearchParams } from "next/navigation";

// get BNB token image url from coingecko
const tokenAImageUrl = `https://assets.coingecko.com/coins/images/825/small/binance-coin-logo.png?1547034615`;
// get ETF token image url from coingecko
const tokenBImageUrl = `https://assets.coingecko.com/coins/images/279/small/ethereum.png?1595348880`;

const PreviewModal = ({
  amountOfTokenA,
  amountOfTokenB,
  maxPrice,
  minPrice,
  feeTier,
  onAdd,
  onClose,
}: PreviewModalProps) => {
  const params = useParams();
  const searchParams = useSearchParams();
  const tokenA = params.token as string;
  const tokenB = searchParams.get("t") as string;

  return (
    <Modal
      open
      width={420}
      title="Add Liquidity"
      onCancel={onClose}
      footer={null}
    >
      <S.PreviewModal>
        <S.PairTokens>
          <S.TokenSymbolsWrapper>
            <img src={tokenAImageUrl} alt="BNB" />
            <img src={tokenBImageUrl} alt="ETF" />
          </S.TokenSymbolsWrapper>
          <div>{`${tokenA}/${tokenB}`}</div>
        </S.PairTokens>
        <S.Box>
          <S.Token>
            <S.TokenInfo>
              <img
                src="https://assets.coingecko.com/coins/images/825/small/binance-coin-logo.png?1547034615"
                alt="BNB"
              />
              {tokenA}
            </S.TokenInfo>
            <S.TokenValue>{amountOfTokenA}</S.TokenValue>
          </S.Token>
          <S.Token>
            <S.TokenInfo>
              <img src={tokenBImageUrl} alt="BNB" />
              {tokenB}
            </S.TokenInfo>
            <S.TokenValue>{amountOfTokenB}</S.TokenValue>
          </S.Token>
          <S.Fee>
            <span>Fee tier</span>
            <span>{feeTier}%</span>
          </S.Fee>
        </S.Box>

        <S.MinMaxPriceWrapper>
          <S.MinMaxPrice>
            <span>Min Price</span>
            <span>{minPrice}</span>
          </S.MinMaxPrice>
          <S.MinMaxPrice>
            <span>Max Price</span>
            <span>{maxPrice}</span>
          </S.MinMaxPrice>
        </S.MinMaxPriceWrapper>

        <Button type="primary" onClick={onAdd}>
          Add
        </Button>
      </S.PreviewModal>
    </Modal>
  );
};

export default PreviewModal;
