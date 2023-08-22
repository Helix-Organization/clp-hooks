import styled from "styled-components";

export const AddLiquidity = styled.div`
  display: flex;
  flex-direction: column;
  margin-top: 100px;
  width: 100%;
  max-width: 640px;

  background-color: #fff;
  border-radius: 16px;
  box-shadow: rgba(0, 0, 0, 0.01) 0px 0px 1px, rgba(0, 0, 0, 0.04) 0px 4px 8px,
    rgba(0, 0, 0, 0.04) 0px 16px 24px, rgba(0, 0, 0, 0.01) 0px 24px 32px;
`;

export const Header = styled.div`
  display: flex;
  align-items: center;
  width: 100%;
  height: 60px;
  padding: 0 20px;
  border-bottom: 1px solid var(--border-color);

  h1 {
    font-size: 1.2rem;
  }

  svg {
    cursor: pointer;
  }
`;

export const HeaderItem = styled.div`
  flex: 1 1 0%;

  h1 {
    text-align: center;
  }
`;

export const Contents = styled.div`
  display: flex;
  gap: 20px;
  padding: 20px;
`;

export const SubTitle = styled.h2`
  font-size: 0.9rem;
  font-weight: 700;
  color: #222;
  margin-bottom: 1rem;
`;

export const ContentsItem = styled.div`
  display: flex;
  flex-direction: column;
  flex: 1 1 0%;
`;

export const TokenButton = styled.div`
  display: flex;
  align-items: center;
  justify-content: space-between;
  width: 100%;
  height: 2rem;
  padding: 0 0.5rem;
  border-radius: 10px;
  background-color: var(--secondary-color);
  cursor: pointer;
  transition: filter 0.2s ease-in-out;
  border: 1px solid var(--border-color);

  .arrow {
    width: 0.8rem;
    height: 0.8rem;
  }

  &:hover {
    filter: brightness(0.9);
  }
`;

export const TokenButtonWrapper = styled.div`
  display: flex;
  gap: 8px;
  margin-bottom: 10px;
`;

export const FeeTier = styled.div`
  display: flex;
  padding: 1rem;
  border: 1px solid var(--border-color);
  border-radius: 10px;
  margin-bottom: 20px;

  font-size: 0.8rem;
  font-weight: 700;
`;

export const DepositAmountInputWrapper = styled.div`
  display: flex;
  flex-direction: column;
  gap: 5px;
`;

export const PreviewButton = styled.button`
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 0.6rem;
  width: 100%;
  color: #fff;
  border-radius: 10px;
  background-color: var(--primary-color);
  outline: none;
  border: none;
  cursor: pointer;
  transition: filter 0.2s ease-in-out;

  &:hover {
    filter: brightness(0.9);
  }
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
