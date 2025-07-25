const { execSync } = require("child_process");

if(process.platform === "linux") {
    try {
        const cflags = execSync(`pkg-config --cflags gtk+-3.0`).toString().trim();
        let flags = cflags.split(" ").filter((f) => f.startsWith("-I"));
        flags = flags.map((f) => f.replace("-I", ""));
        
        for(let flag of flags) {
            console.log(flag);
        }
    } catch(e) {}
}