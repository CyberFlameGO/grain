open Grain_tests.TestFramework;
open Grain_tests.Runner;

describe("linking", ({test}) => {
  let assertRun = makeRunner(test);
  let assertRunError = makeErrorRunner(test);
  let assertWasiPolyfillRun =
    makeRunner(
      ~setup=
        () => {
          Grain_utils.Config.wasi_polyfill :=
            Some("test/input/wasiPolyfill.gr")
        },
      test,
    );

  assertRun("link_simple", {|print("Hello, world!")|}, "Hello, world!\n");
  assertRunError(
    "link_exception",
    {|exception BadError(Number, String); let _ = throw BadError(5, "foo")|},
    {|BadError\(5, "foo"\)|},
  );
  assertRun(
    "link_import",
    {|import List from "list"; print(List.map(n => n + 1, [1, 2, 3]))|},
    "[2, 3, 4]\n",
  );
  // --wasi-polyfill
  assertWasiPolyfillRun("wasi_polyfill", {|print("foo")|}, "foo\nfoo\nfoo\n");
});
