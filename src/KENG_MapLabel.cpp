#include "KENG_MapLabel.hpp"

namespace KENG {
        MapLabel::MapLabel(llui id, f32 x, f32 y, f32 width, f32 height, f32 theta) {
            dirty = true;
            this->id = id;
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
            this->theta = theta;
        }
        MapLabel::~MapLabel(void) {}

        bool MapLabel::IsDirty(void) {
            return dirty;
        }

        void MapLabel::Mark(void) {
            dirty = true;
        }

        void MapLabel::Unmark(void) {
            dirty = false;
        }

        llui MapLabel::Id(void) const { return id; }

        f32 MapLabel::X(void) const { return x; }
        f32 MapLabel::Y(void) const { return y; }
        
        f32 MapLabel::Width(void) const { return width; }
        f32 MapLabel::Height(void) const { return height; }
        
        f32 MapLabel::Theta(void) const { return theta; }

        // Only used here
        static LabelRect FindLargestRectangle(std::vector<ui8>& mask, i32 width, i32 height) {
            LabelRect best;

            std::vector<i32> heights(width, 0);

            for (i32 y = 0 ; y < height ; ++y) {
                for (i32 x = 0 ; x < width ; ++x) {
                    if (mask[y * width + x]) {
                        heights[x]++;
                    } else {
                        heights[x] = 0;
                    }
                }

                std::vector<i32> stack;

                for (i32 x = 0 ; x <= width ; ++x) {
                    const i32 current = (x == width) ? 0 : heights[x];

                    while (!stack.empty() && heights[stack.back()] > current) {
                        const i32 h = heights[stack.back()];
                        
                        stack.pop_back();

                        const i32 left = stack.empty() ? 0 : stack.back() + 1;

                        const i32 w = x - left;
                        const i32 area = w * h;

                        if (area > best.area) {
                            best.area = area;
                            best.x = left;
                            best.y = y - h + 1;
                            best.width = w;
                            best.height = h;
                        }
                    }

                    stack.push_back(x);
                }
            }

            return best;
        }

        std::vector<MapLabel> BuildProvinceMapLabelsCPU(const char* fpath, ProvinceRegistry& preg, FT_Face face) {
            std::vector<MapLabel> labels;

            i32 imgW, imgH, channels;

            ui8* image = stbi_load(fpath, &imgW, &imgH, &channels, 3);
            if (!image) return labels;

            std::vector<Province> provinces = preg.Provinces();
            
            std::unordered_map<ui32, llui> colorLookup;

            for (llui i = 0 ; i < provinces.size() ; ++i) {
                colorLookup[provinces.at(i).Color()] = provinces.at(i).Id() - 1;
            }

            std::vector<PPD> provData(provinces.size());

            for (i32 y = 0 ; y < imgH ; ++y) {
                for (i32 x = 0 ; x < imgW ; ++x) {
                    const llui idx = (y * imgW + x) * 3;

                    // TODO: Replace with PackRGB...
                    const ui32 color = 
                        (static_cast<ui32>(image[idx])     << 16) |
                        (static_cast<ui32>(image[idx + 1]) << 8)  |
                        (static_cast<ui32>(image[idx + 2]));
                        
                    auto it = colorLookup.find(color);
                    
                    if (it == colorLookup.end()) continue;
                    
                    const llui provIdx = it->second;
                    PPD& data = provData[provIdx];
                    
                    data.pixels.push_back(
                        static_cast<ui32>(y * imgW +x)
                    );
                    
                    data.minX = std::min(data.minX, x);
                    data.minY = std::min(data.minY, y);
                    data.maxX = std::max(data.maxX, x);
                    data.maxY = std::max(data.maxY, y);
                }
            }

            // Province geometry is calculated,
            // will calculate positions and dimensions later

            // Process each province independently.
            for (llui i = 0 ; i < provinces.size() ; ++i) {
                Province& province = provinces[i];
                const PPD& d = provData[i];
    
                if (d.maxX < 0) continue;
    
                const int boxW = d.maxX - d.minX + 1;
                const int boxH = d.maxY - d.minY + 1;
    
                std::vector<ui8> mask(
                    static_cast<llui>(boxW) * boxH,
                    0
                );
    
                // Build binary province mask.
                for (int y = d.minY; y <= d.maxY; ++y) {
                    for (int x = d.minX; x <= d.maxX; ++x) {
    
                        const llui p = (y * imgW + x) * 3;
    
                        // TODO: Use PackRGB(...)
                        const ui32 color =
                            (static_cast<ui32>(image[p]) << 16) |
                            (static_cast<ui32>(image[p + 1]) << 8) |
                             static_cast<ui32>(image[p + 2]);
    
                        if (color == province.Color()) {
                            mask[
                                (y - d.minY) * boxW +
                                (x - d.minX)
                            ] = 1;
                        }
                    }
                }
    
                // Find largest rectangle.
                LabelRect rect = FindLargestRectangle(mask, boxW, boxH);
    
                if (rect.area == 0) continue;
    
                // Measure actual text dimensions.
                auto [textW, textH] = OGL_MeasureText(face, province.Name());
    
                if (textW > rect.width || textH > rect.height) continue;
    
                // Centre text within available rectangle.
                const f32 x = static_cast<f32>(d.minX + rect.x) +(rect.width - textW) * 0.5f;
    
                const f32 y = static_cast<f32>(d.minY + rect.y) + (rect.height - textH) * 0.5f;
    
                labels.emplace_back(
                    static_cast<ui32>(province.Id()),
                    x,
                    y,
                    textW,
                    textH,
                    0.0f
                );
            }

            stbi_image_free(image);

            return labels;
        }

        void RenderProvinceMapLabels(OGL_Object& obj, const std::vector<MapLabel> labels, ProvinceRegistry& preg, glm::vec3 color, std::map<char, OGL_Character>& characters) {
            for (const MapLabel& label : labels) {
                Province& province = preg.GetProvince(
                    static_cast<llui>(label.Id())
                );
    
                OGL_RenderTextR(obj, province.Name(), label.X(), label.Y(), label.Theta(), 1.0f, color, characters);
            }
        }
} // KENG