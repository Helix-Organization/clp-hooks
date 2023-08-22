"use client";

import * as S from "./Main.styled";
import { MainProps } from "./Main.types";

const Main = ({ children }: MainProps) => {
  return <S.Main>{children}</S.Main>;
};

export default Main;
