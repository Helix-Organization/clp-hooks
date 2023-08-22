export interface ProcessingModalProps {
  amountOfTokenA?: string;
  amountOfTokenB?: string;
  status: "pending" | "success" | "error";
  onClose: () => void;
}
