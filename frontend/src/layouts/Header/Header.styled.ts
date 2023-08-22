import styled from "styled-components";

export const Header = styled.header`
  display: flex;
  flex-flow: row;
  width: 100%;
  -webkit-box-pack: justify;
  justify-content: space-between;
  position: fixed;
  top: 0px;
  z-index: 1000;
`;

export const Nav = styled.nav`
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 20px;
  width: 100%;
  height: 72px;
  z-index: 2;
`;

export const IconWrapper = styled.div``;
