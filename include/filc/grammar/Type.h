/**
 * MIT License
 *
 * Copyright (c) 2024-Present Kevin Traini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef FILC_TYPE_H
#define FILC_TYPE_H

#include <string>
#include <memory>

namespace filc {
class AbstractType {
  public:
    [[nodiscard]] virtual auto getName() const noexcept -> std::string = 0;

    [[nodiscard]] virtual auto getDisplayName() const noexcept -> std::string = 0;

  protected:
    AbstractType() = default;
};

class Type final: public AbstractType {
  public:
    explicit Type(std::string name);

    [[nodiscard]] auto getName() const noexcept -> std::string override;

    [[nodiscard]] auto getDisplayName() const noexcept -> std::string override;

  private:
    std::string _name;
};

class PointerType final: public AbstractType {
  public:
    explicit PointerType(std::shared_ptr<AbstractType> pointed_type);

    [[nodiscard]] auto getName() const noexcept -> std::string override;

    [[nodiscard]] auto getDisplayName() const noexcept -> std::string override;

  private:
    std::shared_ptr<AbstractType> _pointed_type;
};

class AliasType final: public AbstractType {
  public:
    AliasType(std::string name, std::shared_ptr<AbstractType> aliased_type);

    [[nodiscard]] auto getName() const noexcept -> std::string override;

    [[nodiscard]] auto getDisplayName() const noexcept -> std::string override;

  private:
    std::string _name;
    std::shared_ptr<AbstractType> _aliased_type;
};
}

auto operator==(const std::shared_ptr<filc::AbstractType> &a, const std::shared_ptr<filc::AbstractType> &b) -> bool;
auto operator!=(const std::shared_ptr<filc::AbstractType> &a, const std::shared_ptr<filc::AbstractType> &b) -> bool;

#endif // FILC_TYPE_H
