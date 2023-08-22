import { createGlobalStyle, css } from "styled-components";
import { reset } from "./reset";

export const variables = css`
  :root {
    --primary-color: #0066ff;
    --secondary-color: #4ad8ff1a;
    --border-color: #e5e5e5;
  }
`;

export const GlobalStyle = createGlobalStyle`
  ${reset}

  
  body {
    font-size: 16px;
  }
  
  * {
    box-sizing: border-box;
  }

  a {
    text-decoration: none;
  }

  ${variables}
`;
