import styled from "styled-components";

export const Liquidity = styled.div`
  margin-top: 100px;
  width: 100%;
  max-width: 640px;
`;

export const Title = styled.h1`
  font-size: 2rem;
  margin-bottom: 2rem;
`;

export const AddLiquidityBox = styled.div`
  display: flex;
  flex-direction: column;
  gap: 20px;
  align-items: center;
  justify-content: center;
  padding: 3rem 10rem;
  text-align: center;

  background-color: #fff;
  border-radius: 16px;
  box-shadow: rgba(0, 0, 0, 0.01) 0px 0px 1px, rgba(0, 0, 0, 0.04) 0px 4px 8px,
    rgba(0, 0, 0, 0.04) 0px 16px 24px, rgba(0, 0, 0, 0.01) 0px 24px 32px;
`;

export const AddLiquidityButton = styled.button`
  display: flex;
  align-items: center;
  justify-content: center;
  height: 36px;
  border-radius: 10px;
  background-color: var(--primary-color);
  color: #fff;
  outline: none;
  border: none;
  padding: 0 20px;
  cursor: pointer;
  transition: filter 0.2s ease-in-out;
  font-size: 1.2rem;

  &:hover {
    filter: brightness(0.9);
  }
`;
