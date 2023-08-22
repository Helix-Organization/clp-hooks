export interface PreviewModalProps {
  amountOfTokenA?: string;
  amountOfTokenB?: string;
  minPrice?: number;
  maxPrice?: number;
  feeTier: string;
  onAdd: () => void;
  onClose: () => void;
}
