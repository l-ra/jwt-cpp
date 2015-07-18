// Copyright (c) 2015 Erwin Jansen
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#ifndef SRC_VALIDATORS_CLAIMS_CLAIMVALIDATOR_H_
#define SRC_VALIDATORS_CLAIMS_CLAIMVALIDATOR_H_

#include <jansson.h>
#include <string>
#include <vector>
#include <memory>

class ClaimValidator {
 public:
  virtual ~ClaimValidator() {}
  virtual bool IsValid(const json_t *claimset) const = 0;
  virtual std::string toJson() const = 0;

  inline const char* property() const { return property_; }

 protected:
  explicit ClaimValidator(const char* property) : property_(property) {}
  const char* property_;
};

class AllClaimValidator : public ClaimValidator {
 public:
  AllClaimValidator(const ClaimValidator *const *lstClaims, const size_t numClaims);
  explicit AllClaimValidator(std::vector<ClaimValidator*> validators);
  bool IsValid(const json_t *claimset) const override;
  std::string toJson() const override;

 private:
  std::vector<ClaimValidator*> validators_;
};

class OptionalClaimValidator : public ClaimValidator {
 public:
  explicit OptionalClaimValidator(const ClaimValidator *inner);
  bool IsValid(const json_t *claimset) const override;
  std::string toJson() const override;

 private:
  const ClaimValidator *inner_;
};

class AnyClaimValidator :  public ClaimValidator {
 public:
  AnyClaimValidator(const ClaimValidator *const *lstClaims, const size_t numClaims);
  explicit AnyClaimValidator(std::vector<ClaimValidator*> validators);
  bool IsValid(const json_t *claimset) const override;
  std::string toJson() const override;

 private:
  std::vector<ClaimValidator*> validators_;
};

typedef std::unique_ptr<ClaimValidator> claim_ptr;
#endif  // SRC_VALIDATORS_CLAIMS_CLAIMVALIDATOR_H_