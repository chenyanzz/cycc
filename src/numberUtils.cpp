bool isNumSuffix(const char c) {
	return (c == 'L') || (c == 'l') || (c == 'F') || (c == 'f') || (c == 'u') || (c == 'U');
}

bool isDecDigit(const char c) {
	return c >= '0' && c <= '9';
}

bool isHexDigit(const char c) {
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}