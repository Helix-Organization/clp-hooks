export interface DepositAmountInputProps {
  token: string;
  value?: string;
  onChange: (value: string) => void;
  balance: number;
  dollarValue?: number;
}
