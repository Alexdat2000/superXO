function getCookie(name) {
  const value = "; " + document.cookie;
  const parts = value.split("; " + name + "=");
  if (parts.length === 2) return parts.pop().split(";").shift();
  return null;
}

function generateRandomId(length) {
  const chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  let result = "";
  for (let i = 0; i < length; i++) {
    result += chars.charAt(Math.floor(Math.random() * chars.length));
  }
  return result;
}

export function EnsureUserId() {
  if (!getCookie("userId")) {
    const newId = generateRandomId(10);
    document.cookie =
      "userId=" + newId + "; path=/; max-age=" + 60 * 60 * 24 * 365 * 10;
  }
}
