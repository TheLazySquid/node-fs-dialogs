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
    if(!defaultPath) return;
    return resolve(defaultPath);
}

function pickFile(options = {}) {
    const filters = formatFilters(options.filters)
    const defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickFile(filters, defaultPath, options.title, options.acceptLabel, options.cancelLabel);
}

function pickFiles(options = {}) {
    const filters = formatFilters(options.filters)
    const defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickFiles(filters, defaultPath, options.title, options.acceptLabel, options.cancelLabel);
}

function pickDirectory(options = {}) {
    const defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickDirectory(defaultPath, options.title, options.acceptLabel, options.cancelLabel);
}

function pickDirectories(options = {}) {
    const defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickDirectories(defaultPath, options.title, options.acceptLabel, options.cancelLabel);
}

function pickSaveFile(options = {}) {
    const filters = formatFilters(options.filters);
    const defaultPath = formatDefaultPath(options.defaultPath);

    return addon.pickSaveFile(filters, defaultPath, options.defaultName, options.title, options.acceptLabel, options.cancelLabel);
}

module.exports.pickFile = pickFile;
module.exports.pickFiles = pickFiles;
module.exports.pickDirectory = pickDirectory;
module.exports.pickDirectories = pickDirectories;
module.exports.pickSaveFile = pickSaveFile;