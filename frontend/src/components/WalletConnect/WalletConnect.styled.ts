import styled from "styled-components";

export const WalletConnect = styled.div`
  display: flex;
  align-items: center;
  padding: 0px 24px;
  color: var(--primary-color);
  border: 1px solid var(--primary-color);
  height: 40px;
  border-radius: 28px;
  font-size: 22px;
  transition: all 0.2s ease 0s;
  cursor: pointer;

  &&:hover {
    color: #fff;
    background-color: var(--primary-color);
  }
`;
