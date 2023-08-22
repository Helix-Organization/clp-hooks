"use client";

import Link from "next/link";
import * as S from "./Liquidity.styled";
import { LiquidityProps } from "./Liquidity.types";
import Drawer from "@/assets/svgs/drawer.svg";

const Liquidity = () => {
  return (
    <S.Liquidity>
      <S.Title>Liquidity</S.Title>
      <S.AddLiquidityBox>
        <Drawer />
        <p>Your active V3 liquidity positions will appear here.</p>
        <Link href="/add/XRP">
          <S.AddLiquidityButton>Add Liquidity</S.AddLiquidityButton>
        </Link>
      </S.AddLiquidityBox>
    </S.Liquidity>
  );
};

export default Liquidity;
