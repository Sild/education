import * as fs from 'fs';
import process from "process";
import {Cell} from "ton-core";
import {compileFunc} from "@ton-community/func-js";

async function compileScript() {
    console.log("=====")
    const compileResult = await compileFunc({
        targets: ["./contracts/main.fc"],
        sources: (x) => fs.readFileSync(x, "utf8"),
    });
    console.log("compiling...")

    
    if (compileResult.status === "error") {
        console.log("error: " + compileResult.message);
        process.exit(1);
    }
    console.log("compiled")


    const hexArtifact = `build/main.compiled.json`;

    fs.writeFileSync(
        hexArtifact,
        JSON.stringify({
            hex: Cell.fromBoc(Buffer.from(compileResult.codeBoc, "base64"))[0].toBoc().toString("hex")
        })
        
    )
    console.log("saved to: " + hexArtifact);
}

compileScript()
