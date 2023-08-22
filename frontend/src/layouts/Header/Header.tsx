"use client";

import { WalletConnect } from "@/components/WalletConnect";
import * as S from "./Header.styled";
import { HeaderProps } from "./Header.types";
import Link from "next/link";

const Header = () => {
  return (
    <S.Header>
      <S.Nav>
        <Link href="/">
          <S.IconWrapper>🖤</S.IconWrapper>
        </Link>
        <WalletConnect />
      </S.Nav>
    </S.Header>
  );
};

export default Header;
