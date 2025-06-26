const addon = require("../build/Release/node-fs-dialogs");
const { resolve } = require("path");

function formatFilters(filters = []) {
    if(!Array.isArray(filters)) {
        filters = [filters];
    }

    for(let filter of filters) {
        if(Array.isArray(filter.extensions)) {
            filter.extensions = filter.extensions.join(",");
        }
    }

    return filters;
}

function formatDefaultPath(defaultPath) {
    if(!defaultPath) return "";
    return resolve(defaultPath);
}

function pickFile(options = {}) {
    let filters = formatFilters(options.filters)
    let defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickFile(filters, defaultPath);
}

function pickFiles(options = {}) {
    let filters = formatFilters(options.filters)
    let defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickFiles(filters, defaultPath);
}

function pickDirectory(options = {}) {
    let defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickDirectory(defaultPath);
}

function pickDirectories(options = {}) {
    let defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickDirectories(defaultPath);
}

function pickSaveFile(options = {}) {
    let filters = formatFilters(options.filters);
    let defaultPath = formatDefaultPath(options.defaultPath);
    let defaultName = options.defaultName ?? "";

    return addon.pickSaveFile(filters, defaultPath, defaultName);
}

module.exports.pickFile = pickFile;
module.exports.pickFiles = pickFiles;
module.exports.pickDirectory = pickDirectory;
module.exports.pickDirectories = pickDirectories;
module.exports.pickSaveFile = pickSaveFile;