"use client";

import BackArrow from "@/assets/svgs/back-arrow.svg";
import DownArrow from "@/assets/svgs/down-arrow.svg";
import { useParams, useRouter, useSearchParams } from "next/navigation";
import * as S from "./AddLiquidity.styled";

import { Button, Slider } from "antd";
import Link from "next/link";
import { useState } from "react";
import { DepositAmountInput } from "../DepositAmountInput";
import { SelectTokenModal } from "../SelectTokenModal";
import { PreviewModal } from "../PreviewModal";
import { ProcessingModal } from "../ProcessingModal";

const AddLiquidity = () => {
  const params = useParams();
  const searchParams = useSearchParams();
  const tokenA = params.token as string;
  const tokenB = searchParams.get("t") as string;
  const router = useRouter();

  const [amountOfTokenA, setAmountOfTokenA] = useState<string>();
  const [amountOfTokenB, setAmountOfTokenB] = useState<string>();
  const [minPrice, setMinPrice] = useState<number>(0);
  const [maxPrice, setMaxPrice] = useState<number>(1);
  const [isOpenSelectTokenAModal, setIsOpenSelectTokenAModal] = useState(false);
  const [isOpenSelectTokenBModal, setIsOpenSelectTokenBModal] = useState(false);
  const [isOpenPreviewModal, setIsOpenPreviewModal] = useState(false);
  const [isOpenProcessingModal, setIsOpenProcessingModal] = useState(false);
  const [processingStatus, setProcessingStatus] = useState<
    "pending" | "success" | "error"
  >("pending");

  const handleTokenASelect = (token: string) => {
    router.push(`/add/${token}${tokenB ? `?t=${tokenB}` : ""}`);
  };

  const handleTokenBSelect = (token: string) => {
    router.push(`/add/${tokenA}?t=${token}`);
  };

  const handlePriceRangeChange = (value: number[]) => {
    setMinPrice(value[0]);
    setMaxPrice(value[1]);
  };

  const handleLiquidityAddButtonClick = async () => {
    try {
      setIsOpenPreviewModal(false);
      setIsOpenProcessingModal(true);
      await new Promise((resolve) => setTimeout(resolve, 2000));
      setProcessingStatus("success");
    } catch (error) {
      setProcessingStatus("error");
    }
  };

  return (
    <>
      <S.AddLiquidity>
        <S.Header>
          <S.HeaderItem>
            <Link href="/">
              <BackArrow />
            </Link>
          </S.HeaderItem>
          <S.HeaderItem>
            <h1>Add Liquidity</h1>
          </S.HeaderItem>
          <S.HeaderItem />
        </S.Header>

        <S.Contents>
          <S.ContentsItem>
            <S.SubTitle>Select Pair</S.SubTitle>
            <S.TokenButtonWrapper>
              <S.TokenButton onClick={() => setIsOpenSelectTokenAModal(true)}>
                {tokenA}
                <DownArrow className="arrow" />
              </S.TokenButton>
              <S.TokenButton onClick={() => setIsOpenSelectTokenBModal(true)}>
                {tokenB ? (
                  <>
                    {tokenB}
                    <DownArrow className="arrow" />
                  </>
                ) : (
                  <>
                    Select a token
                    <DownArrow className="arrow" />
                  </>
                )}
              </S.TokenButton>
            </S.TokenButtonWrapper>
            <S.FeeTier>
              <span>1% fee tier</span>
            </S.FeeTier>

            <S.SubTitle>Deposit Amounts</S.SubTitle>

            <S.DepositAmountInputWrapper>
              <DepositAmountInput
                value={amountOfTokenA}
                token={tokenA}
                balance={0}
                dollarValue={12.5}
                onChange={(value) => setAmountOfTokenA(value)}
              />
              {tokenB && (
                <DepositAmountInput
                  value={amountOfTokenB}
                  token={tokenB}
                  balance={0}
                  dollarValue={12.5}
                  onChange={(value) => setAmountOfTokenB(value)}
                />
              )}
            </S.DepositAmountInputWrapper>
          </S.ContentsItem>
          <S.ContentsItem>
            <S.SubTitle>Set Price Range</S.SubTitle>

            <Slider
              range
              defaultValue={[0, 1]}
              min={0}
              max={1}
              step={0.01}
              disabled={!tokenB}
              onChange={handlePriceRangeChange}
            />

            <S.MinMaxPriceWrapper>
              <S.MinMaxPrice>
                <span>Min Price</span>
                <span>{minPrice}</span>
              </S.MinMaxPrice>
              <S.MinMaxPrice>
                <span>Max Price</span>
                <span>{maxPrice}</span>
              </S.MinMaxPrice>
            </S.MinMaxPriceWrapper>

            <Button
              type="primary"
              disabled={!amountOfTokenA || !amountOfTokenB}
              onClick={() => setIsOpenPreviewModal(true)}
            >
              {!amountOfTokenA || !amountOfTokenB
                ? "Enter an Amounts"
                : "Preview"}
            </Button>
          </S.ContentsItem>
        </S.Contents>
      </S.AddLiquidity>
      {isOpenSelectTokenAModal && (
        <SelectTokenModal
          onSelect={handleTokenASelect}
          tokens={[]}
          onClose={() => setIsOpenSelectTokenAModal(false)}
        />
      )}
      {isOpenSelectTokenBModal && (
        <SelectTokenModal
          onSelect={handleTokenBSelect}
          tokens={[]}
          onClose={() => setIsOpenSelectTokenBModal(false)}
        />
      )}
      {isOpenPreviewModal && (
        <PreviewModal
          amountOfTokenA={amountOfTokenA}
          amountOfTokenB={amountOfTokenB}
          minPrice={minPrice}
          maxPrice={maxPrice}
          feeTier="1"
          onAdd={handleLiquidityAddButtonClick}
          onClose={() => setIsOpenPreviewModal(false)}
        />
      )}
      {isOpenProcessingModal && (
        <ProcessingModal
          status={processingStatus}
          onClose={() => setIsOpenProcessingModal(false)}
        />
      )}
    </>
  );
};

export default AddLiquidity;
