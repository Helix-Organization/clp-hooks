import * as S from "./DepositAmountInput.styled";
import { DepositAmountInputProps } from "./DepositAmountInput.types";

const DepositAmountInput = ({
  balance,
  value,
  dollarValue,
  onChange,
  token,
}: DepositAmountInputProps) => {
  return (
    <S.DepositAmountInput>
      <S.Flex>
        <S.AmountInput
          placeholder="0"
          inputMode="decimal"
          autoComplete="off"
          type="text"
          pattern="^[0-9]*[.,]?[0-9]*$"
          value={value}
          onChange={(event) => {
            const regex = /^[0-9]*[.,]?[0-9]*$/;
            if (regex.test(event.target.value)) {
              onChange(event.target.value);
            }
          }}
        />
        <S.SelectedToken>{token}</S.SelectedToken>
      </S.Flex>
      <S.Flex>
        <S.DollarValue>{dollarValue ? `$${dollarValue}` : "-"}</S.DollarValue>
        <S.Balance>Balance: {balance}</S.Balance>
      </S.Flex>
    </S.DepositAmountInput>
  );
};

export default DepositAmountInput;
