String getClassification(float ppm) {
    if (ppm < 30) return "Not Sweet";
    if (ppm < 65) return "Mildly Sweet";
    return "Sweet";
}
