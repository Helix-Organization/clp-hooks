import { Button, Modal } from "antd";
import * as S from "./ProcessingModal.styled";
import { ProcessingModalProps } from "./ProcessingModal.types";
import { CheckCircleOutlined, LoadingOutlined } from "@ant-design/icons";
import { useParams, useSearchParams } from "next/navigation";

const ProcessingModal = ({
  status,
  amountOfTokenA,
  amountOfTokenB,
  onClose,
}: ProcessingModalProps) => {
  const params = useParams();
  const searchParams = useSearchParams();
  const tokenA = params.token as string;
  const tokenB = searchParams.get("t") as string;

  return (
    <Modal open width={420} footer={null}>
      <S.ProcessingModal>
        {status === "pending" && (
          <>
            <LoadingOutlined style={{ fontSize: 50 }} />
            <h3>Waiting for confirmation</h3>
            <p>{`Supplying ${amountOfTokenA} ${tokenA} and ${amountOfTokenB} ${tokenB}`}</p>
          </>
        )}
        {status === "success" && (
          <>
            <CheckCircleOutlined style={{ fontSize: 50 }} />
            <h3>Transaction submitted</h3>
            <Button type="primary" onClick={onClose}>
              Close
            </Button>
          </>
        )}
      </S.ProcessingModal>
    </Modal>
  );
};

export default ProcessingModal;
