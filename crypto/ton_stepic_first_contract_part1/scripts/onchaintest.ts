import { Address, Cell, contractAddress, toNano } from "ton-core";
import { hex } from "../build/main.compiled.json"
import { getHttpV4Endpoint } from "@orbs-network/ton-access";
import {TonClient4} from "ton";
import qs from "qs";
import qrcode from "qrcode-terminal"

import dotenv from "dotenv"
dotenv.config();

async function onchainTestScript() {
    const codeCell = Cell.fromBoc(Buffer.from(hex, "hex"))[0];
    const dataCell = new Cell();

    const address = contractAddress(0, {
        code: codeCell,
        data: dataCell,
    });

    const endpoint = await getHttpV4Endpoint({
        network: process.env.TESTNET ? "testnet" : "mainnet",
    });
    const client4 = new TonClient4 ({endpoint})

    const latestBlock = await client4.getLastBlock();
    let status = await client4.getAccount(latestBlock.last.seqno, address);

    if (status.account.state.type !== "active") {
        console.log("Contract is not active")
        return; 
    }

    let link = `https://${process.env.TESTNET ? "test." : ""}tonhub.com/transfer/` + address.toString({
        testOnly: process.env.TESTNET ? true: false,
    }) + "?" + qs.stringify({
        text: "Test transaction",
        amount: toNano('0.05').toString(10),
    });

    qrcode.generate(link, {small: true}, (code) => {
        console.log(code);
    })

    let recentSenderArchive: Address;

    setInterval(async () => {
        const latestBlock = await client4.getLastBlock();
        const {exitCode, result} = await client4.runMethod(
            latestBlock.last.seqno,
            address,
            "get_the_latest_sender",
        );
        if (exitCode!== 0) {
            console.log("error: " + result);
            return;
        }
        if (result[0].type !== "slice") {
            console.log("unknown result type: " + result[0].type);
            return;
        }

        let mostRecentSender = result[0].cell.beginParse().loadAddress();

        if (mostRecentSender && mostRecentSender.toString() != recentSenderArchive?.toString()) {
            console.log("new sender: " + mostRecentSender.toString());
            recentSenderArchive = mostRecentSender;
        }
    }, 2000);

}

onchainTestScript()