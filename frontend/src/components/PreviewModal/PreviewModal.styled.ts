import styled from "styled-components";

export const PreviewModal = styled.div`
  display: flex;
  flex-direction: column;
  gap: 10px;
`;

export const PairTokens = styled.div`
  display: flex;
  font-size: 1.3rem;
  font-weight: 500;
`;

export const TokenSymbolsWrapper = styled.div`
  position: relative;
  display: flex;
  align-items: center;
  margin-right: 1rem;

  img {
    padding: 4px;
    width: 1.5rem;
    height: 1.5rem;
    background-color: #efefef;
    border-radius: 50%;
  }

  img:last-child {
    position: absolute;
    left: 0.75rem;
  }
`;

export const Box = styled.div`
  display: flex;
  flex-direction: column;
  padding: 0.5rem;
  border-radius: 10px;
  border: 1px solid var(--border-color);
`;

export const Token = styled.div`
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: 1rem;
  font-weight: 500;
`;

export const TokenInfo = styled.div`
  display: flex;
  align-items: center;
  gap: 8px;

  img {
    width: 1rem;
  }
`;

export const TokenValue = styled.div``;

export const Fee = styled.div`
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding-top: 0.5rem;
  border-top: 1px solid var(--border-color);

  font-size: 0.8rem;
  font-weight: 500;
`;

export const MinMaxPriceWrapper = styled.div`
  display: flex;
  gap: 8px;
  align-items: center;
  margin-bottom: 10px;
`;

export const MinMaxPrice = styled.div`
  display: flex;
  flex-direction: column;
  align-items: center;
  gap: 5px;
  width: 100%;
  border: 1px solid var(--border-color);
  padding: 0.5rem;
  border-radius: 10px;

  span:first-child {
    font-size: 0.8rem;
  }

  span:last-child {
    font-weight: 700;
  }
`;
