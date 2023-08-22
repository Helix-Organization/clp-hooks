import styled from "styled-components";

export const Background = styled.div`
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  pointer-events: none;
  width: 200vw;
  height: 200vh;
  z-index: -1;
  transform: translate(-50vw, -100vh);
  background: radial-gradient(
      100% 100% at 50% 0%,
      rgba(0, 102, 255, 0.55) 0%,
      rgba(255, 255, 255, 0) 100%
    ),
    rgb(255, 255, 255);
`;
