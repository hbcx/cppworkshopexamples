#include <mdspan>
#include <vector>
#include <iostream>
#include <cstddef>

int main() {
    // A tiny 2x3 RGB image: height 2, width 3, 3 channels. Stored flat,
    // row-major and pixel-interleaved: [r,g,b, r,g,b, ...].
    const std::size_t H = 2, W = 3, C = 3;
    std::vector<int> pixels(H * W * C);
    for (std::size_t i = 0; i < pixels.size(); ++i) pixels[i] = static_cast<int>(i);

    std::mdspan<int, std::dextents<std::size_t, 3>> img(pixels.data(), H, W, C);

    std::cout << "rank = " << img.rank() << "\n";   // 3

    // Read the green channel (index 1) of every pixel.
    std::cout << "green channel:\n";
    for (std::size_t y = 0; y < H; ++y) {
        for (std::size_t x = 0; x < W; ++x) std::cout << img[y, x, 1] << ' ';
        std::cout << '\n';
    }

    // Brighten the blue channel (index 2) of the top-left pixel.
    img[0, 0, 2] += 100;
    std::cout << "pixels[2] after brighten -> " << pixels[2] << "\n";  // 2 -> 102
}
