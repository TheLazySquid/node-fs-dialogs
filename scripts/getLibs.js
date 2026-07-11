const { execSync } = require("child_process");

if(process.platform === "linux" || process.platform === "darwin") {
    try {
        const libs = execSync(`pkg-config --libs gtk+-3.0`).toString().trim();
        
        for(let lib of libs.split(" ")) {
            console.log(lib);
        }
    } catch(e) {}
}