"use client";

import { Button, Form, Input, Modal } from "antd";
import * as S from "./WalletConnect.styled";
import { WalletConnectProps } from "./WalletConnect.types";
import { useState } from "react";

const WalletConnect = () => {
  const [isModalOpen, setIsModalOpen] = useState(false);

  const handleConnect = ({
    publicKey,
    privateKey,
  }: {
    publicKey: string;
    privateKey: string;
  }) => {
    console.log(publicKey, privateKey);
  };

  return (
    <>
      <Button type="primary" onClick={() => setIsModalOpen(true)}>
        Connect
      </Button>
      {isModalOpen && (
        <Modal
          open
          title="Connect your wallet"
          onCancel={() => setIsModalOpen(false)}
          footer={null}
        >
          <Form
            style={{ marginTop: 40 }}
            onFinish={handleConnect}
            labelCol={{ span: 6 }}
            layout="horizontal"
          >
            <Form.Item
              label="Public Key"
              name="publicKey"
              rules={[{ required: true, message: "Please input public key" }]}
            >
              <Input />
            </Form.Item>
            <Form.Item
              label="Private Key"
              name="privateKey"
              rules={[{ required: true, message: "Please input private key" }]}
            >
              <Input />
            </Form.Item>
            <Form.Item style={{ textAlign: "right" }}>
              <Button type="primary" htmlType="submit">
                Connect
              </Button>
            </Form.Item>
          </Form>
        </Modal>
      )}
    </>
  );
};

export default WalletConnect;
