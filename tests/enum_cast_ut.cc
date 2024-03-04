#include <simple_enum/enum_cast.hpp>
#include "simple_enum_tests.hpp"

namespace simple_enum
  {
static void enum_cast_test()
  {
  "enum_cast success"_test = []
  {
    using enum lorem_ipsum_long;
      {
      auto t1{enum_cast<lorem_ipsum_long>("sit")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == sit);
      }
      {
      auto t1{enum_cast<lorem_ipsum_long>("asteroides")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == asteroides);
      }
      {
      auto t1{enum_cast<lorem_ipsum_long>("eu")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == eu);
      }
      {
      auto t1{enum_cast<strong_typed>("v2")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == strong_typed::v2);
      }
      {
      auto t1{enum_cast<weak::weak_typed_e>("v1")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == weak::weak_typed_e::v1);
      }
      {
      auto t1{enum_cast<weak::weak_typed_e>("v1")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == weak::weak_typed_e::v1);
      }
      {
      auto t1{enum_cast<test::subnamespace::weak_untyped_3_e>("v1")};
      expect(fatal(t1.has_value()));
      expect(t1.value() == test::subnamespace::v1);
      }
  };

  "enum_cast failure"_test = []
  {
    using enum lorem_ipsum_long;
    using enum enum_cast_error;
      {
      auto t1{enum_cast<lorem_ipsum_long>("")};
      expect(fatal(!t1.has_value()));
      expect(t1.error() == invalid_cast);
      }
      {
      auto t1{enum_cast<lorem_ipsum_long>("unknown")};
      expect(fatal(!t1.has_value()));
      expect(t1.error() == invalid_cast);
      }
      {
      auto t1{enum_cast<lorem_ipsum_long>("asteroid")};
      expect(fatal(!t1.has_value()));
      expect(t1.error() == invalid_cast);
      }
      {
      auto t1{enum_cast<lorem_ipsum_long>("steroides")};
      expect(fatal(!t1.has_value()));
      expect(t1.error() == invalid_cast);
      }
      {
      auto t1{enum_cast<weak::weak_typed_e>("steroides")};
      expect(fatal(!t1.has_value()));
      expect(t1.error() == invalid_cast);
      }
      {
      auto t1{enum_cast<weak::weak_typed_e>("v")};
      expect(fatal(!t1.has_value()));
      expect(t1.error() == invalid_cast);
      }
  };
  }
  }  // namespace simple_enum

int main() { simple_enum::enum_cast_test(); }
