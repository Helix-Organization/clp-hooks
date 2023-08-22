# Concentrated Liquidity Pool on XRP Ledger

## Code Overview
hooks is all defined in src/clp.c
other files are only for testing and debugging

## Table of Contents

1. Introduction
   1. What is a Concentrated Liquidity Pool?
   2. Why Use a Concentrated Liquidity Pool?
   3. How it can benefit the XRP Ledger?
2. System Architecture
3. Core Features
   1. AMM
   2. Concentrated Liquidity
   3. LP Tokens and Fees
4. Code Overview
   1. Main Entry Point
   2. Deposit Transaction
   3. Withdraw Transaction
   4. Swap Transaction
5. Conclusion and Future Work

---

## 1. Introduction

### 1.1 What is a Concentrated Liquidity Pool?

A Concentrated Liquidity Pool is an evolutionary step in the Automated Market Maker (AMM) model, offering a more granular control to liquidity providers over the price ranges in which they provide liquidity. Unlike traditional AMMs, where assets are uniformly distributed across all price points, concentrated liquidity pools allow providers to specify certain price intervals for their capital allocation. This means that within these specified price intervals, the liquidity provided can be much denser, leading to tighter spreads and more efficient trading.

Key technical features include:

- **Granular Price Ranges:** Liquidity providers can set specific price bounds, allowing them to target ranges where they anticipate more trading activity.
- **Dynamic Pool Rebalancing:** As market prices move, the pool can be rebalanced to adjust to the new market conditions, ensuring efficient trades.
- **Slippage Control:** The concentrated nature of the liquidity reduces slippage, especially for large trades within the targeted price range.

### 1.2 Why Use a Concentrated Liquidity Pool?

- **Efficient Capital Utilization:** By focusing their capital on specific price ranges, liquidity providers can achieve higher capital efficiency. This means they can potentially earn similar or higher returns compared to traditional AMMs but with less capital at risk.
- **Flexibility:** Concentrated liquidity pools offer dynamic adaptation. Providers aren't locked into a single price range and can adjust their positions based on market insights or algorithmic strategies. This adaptability is crucial, especially in volatile markets.

- **Increased Earnings:** The concentrated nature of the pool ensures that a significant portion of the trades will occur within the chosen price ranges. As a result, liquidity providers can earn a larger share of the trading fees for the capital they've provided.

- **Risk Management:** By limiting exposure to specific price ranges, liquidity providers can manage their risk better, especially in volatile markets where prices can swing dramatically.

### 1.3 How it can benefit the XRP Ledger?

- **Enhanced DEX Capabilities:** The XRP Ledger's decentralized exchange is one of its core features. By introducing concentrated liquidity pools, the XRPL can offer more competitive spreads and better trade execution, making it more attractive to traders and market makers.

- **Attracting More Capital:** The benefits of concentrated liquidity—such as increased returns and better capital efficiency—can incentivize more liquidity providers to participate, thereby increasing the overall liquidity available on the XRPL.

- **Increased Trading Volume:** Enhanced liquidity and reduced slippage can attract more traders to the XRPL, leading to higher trading volumes. This increased activity can, in turn, generate more fees for liquidity providers and make the XRPL a more vibrant trading ecosystem.

- **Interoperability and Integration:** With advanced AMM features, the XRPL can potentially integrate more seamlessly with other blockchain networks and DeFi platforms, bridging liquidity and enabling more complex financial operations.

- **Innovation and Development:** The introduction of concentrated liquidity can spur further innovation on the XRPL, with developers building new tools, analytics platforms, and trading strategies tailored to this new liquidity model.

## 2. System Architecture

The system leverages XRPL's smart contract capability known as "Hooks" to implement a concentrated liquidity pool. The architecture is designed to be both efficient and flexible, catering to the unique requirements of the XRP Ledger and the concentrated liquidity model.

### 2.1 Overview

- **Hooks:** At the heart of the system is XRPL's smart contract feature, Hooks. These are lightweight, efficient, and designed to execute a set of instructions upon receiving a payment transaction.

- **Hook Account:** In our system, the Hook Account is the primary entry point for interacting with the liquidity pool. Users send payment transactions to this account, which then executes the relevant instructions based on the transaction's memo. The Hook Account also stores the state of the liquidity pool, including the amount of each token in the pool, the current price, and other relevant data. This data is updated after each transaction, ensuring that the pool's state is always up to date. The Hook Account also issues LP tokens, which represent LPs' share in the pool.

- **Token Pairs and Price Ranges:** Each account supports one token pair but can accommodate multiple price ranges. These ranges are denoted by numbers from 0 to 255, with each successive range representing a 5% (1.05 times) price difference from its predecessor. This setup essentially means that one account can host up to 256 individual liquidity pools. The price range is designed to protect liquidity providers by limiting their maximum potential loss to just 5%.

### 2.2 Functional Components

- **Deposits:** When a user deposits tokens into a pool, the tokens are locked within the pool, and in return, the user receives LP (Liquidity Provider) tokens. These LP tokens represent the user's share in the pool and can be used to reclaim the deposited tokens later.

- **Withdrawals:** Opposite to deposits, when a user wishes to withdraw their tokens, they return the LP tokens to the pool. In return, they receive their share of the locked tokens based on the amount of LP tokens they provide.

- **Swaps:** Users can swap one type of token for another. The rate at which this swap occurs is determined by the current liquidity and price of the tokens within the chosen price range. A fee is applied to each swap, which is then permanently added to the pool's liquidity, benefiting all liquidity providers.

- **Liquidity States:** The state of the liquidity, such as the amount of each token in the pool and other associated data, is stored in the hook's state. This ensures that the data is both secure and readily accessible for swift operations.

### 2.3 Transaction Mechanics

To interact with a hook, users send a payment transaction to the hook's associated account. This transaction contains two key components in its memo:

- **Transaction Type:** Represented as a single character, it can be 'D' for Deposit, 'W' for Withdraw, and 'S' for Swap.

- **Price Range:** A number between 0 to 255, indicating the specific price range the user wants to interact with.

Both the transaction type and the price range are provided in hexadecimal format, ensuring compatibility and efficient processing on the XRPL.

### 2.4 Fee Structure

A fee is levied on swaps to incentivize liquidity provision and ensure the system's sustainability. This fee is not returned to the users but is instead added permanently to the pool's liquidity. This mechanism not only rewards liquidity providers but also ensures the continuous growth of the pool's depth over time.

This architecture provides a robust framework for implementing concentrated liquidity pools on the XRPL, combining the efficiency of hooks with the flexibility of the concentrated liquidity model.

## 3. Core Features

### 3.1 AMM

- **Algorithmic Price Determination:** Uses mathematical formulas to automatically determine asset prices.
- **Seamless Trades:** Users can trade assets without the need for a counterparty, relying on the liquidity pool.
- **Dynamic Fee Structure:** As trade size increases, the price impact and fees increase, ensuring the system remains balanced.

### 3.2 Concentrated Liquidity

- **Custom Price Ranges:** Liquidity providers can choose specific price ranges to allocate their capital.
- **Optimized Returns:** Capital isn't spread out over the entire curve, leading to optimized returns for liquidity providers.
- **Dynamic Adjustments:** Providers can adjust their price ranges in response to market conditions.

### 3.3 LP Tokens and Fees

- **Tokenized Representation:** LP tokens represent a provider's stake in the pool.
- **Earning Mechanism:** Providers earn fees from trades that happen within their chosen price ranges.
- **Redemption:** LP tokens can be redeemed for the underlying assets, plus any earned fees.

## 4. Code Overview

### 4.1 Main Entry Point

This is the primary function that initializes our Hook on the XRPL, setting up the necessary data structures and preparing for transactions.

### 4.2 Deposit Transaction

- **Capital Allocation:** Allows liquidity providers to add capital to specific price ranges.
- **LP Token Issuance:** Upon deposit, liquidity providers receive LP tokens proportional to their contribution.

### 4.3 Withdraw Transaction

- **Capital Return:** Liquidity providers can withdraw their capital from the pool.
- **LP Token Redemption:** Providers burn LP tokens to withdraw their assets, plus any earned fees.

### 4.4 Swap Transaction

- **Trade Execution:** Users can trade assets through the pool, with prices determined by the AMM formula.
- **Fee Distribution:** A portion of each trade is taken as a fee and distributed to relevant liquidity providers.

## 5. Conclusion and Future Work

In conclusion, as our team delves deeper into the development of our Decentralized Exchange (DEX), the integration of the concentrated liquidity pool stands out as a pivotal feature that promises to revolutionize our platform.

Our future plans encompass the following key enhancements:

1. **Pathfinding Algorithm Enhancement:** We aim to integrate the concentrated liquidity pool into our pathfinding algorithm. This integration will empower users to discover the most favorable exchange rates seamlessly.
2. **Automatic Pool Selection:** A new feature is on the horizon that fetches the internal price. With this feature in place, users can effortlessly engage with the pool that matches the current market price, ensuring a more intuitive trading experience.
3. **Decentralized Fee Rate Voting:** To foster a democratic ecosystem, we plan to introduce a mechanism allowing users to vote on the pool's fee rate using LP Tokens. This initiative ensures that the fee structure remains transparent, fair, and in line with the community's preferences.

By introducing these features, we aim to position our DEX as a state-of-the-art platform that is not only user-friendly but also offers advanced functionalities tailored to the evolving needs of the crypto trading community.
