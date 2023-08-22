import styled from "styled-components";

export const DepositAmountInput = styled.div`
  display: flex;
  flex-direction: column;
  gap: 5px;

  padding: 1rem;
  border-radius: 0.5rem;
  border: 1px solid var(--border-color);
`;

export const AmountInput = styled.input`
  border: none;
  background-color: transparent;
  outline: none;
  font-size: 1.2rem;
  width: 100%;
`;

export const Flex = styled.div`
  display: flex;
  align-items: center;
  justify-content: space-between;
`;

export const SelectedToken = styled.div`
  padding: 0.5rem;
  background-color: var(--secondary-color);
  font-size: 1.1rem;
  font-weight: 500;
  border-radius: 10px;
`;

export const DollarValue = styled.div`
  font-size: 0.7rem;
  color: #333333;
`;

export const Balance = styled.div`
  font-size: 0.8rem;
`;
