import styled from "styled-components";

export const TokenList = styled.ul`
  margin: 0 -24px;
`;

export const TokenItem = styled.li`
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 1rem;
  cursor: pointer;

  img {
    width: 1rem;
    height: auto;
  }

  &:hover {
    background-color: #efefef;
  }
`;
