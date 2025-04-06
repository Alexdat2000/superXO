export function StringToIndex(str) {
    return str.charCodeAt(0) - 65 + (str.charCodeAt(1) - 49) * 9;
}

export function IndexToString(index) {
    return String.fromCharCode(65 + index % 9) + String.fromCharCode(49 + index / 9);
}