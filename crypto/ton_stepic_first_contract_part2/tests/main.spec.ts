import { Cell, Address, toNano } from "ton-core";
import { hex } from "../build/main.compiled.json"
import { Blockchain } from "@ton-community/sandbox";
import { MainContract } from "../wrappers/MainContract";
import "@ton-community/test-utils";

describe("main.fc contract tests", () => {
    it("get proper sender address", async() => {
        const codeCell = Cell.fromBoc(Buffer.from(hex, "hex"))[0];

        const blockchain = await Blockchain.create();
        const initAddress = await blockchain.treasury("initAddress");
        const myContract = blockchain.openContract(
            await MainContract.createFromConfig({number: 0, address: initAddress.address}, codeCell)
        );

        const senderWallet = await blockchain.treasury("sender");

        const sentMessageResult = await myContract.sendInternalMessage(senderWallet.getSender(), toNano("0.05")); // 50*10^6

        expect(sentMessageResult.transactions).toHaveTransaction(
            {
                from: senderWallet.address,
                to: myContract.address,
                success: true,
            }
        )

        const data = await myContract.getData();
        expect(data.recent_sender.toString()).toBe(senderWallet.address.toString());

    });
});