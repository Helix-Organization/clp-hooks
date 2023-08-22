export interface SelectTokenModalProps {
  tokens: {
    logoUrl: string;
    name: string;
  }[];
  onSelect: (token: string) => void;
  onClose: () => void;
}
