/**
 * Copyright (c) 2017 - present Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

open Immutable;
open ReUnit;
open ReUnit.Test;

let test = describe "Iterable" [
  it "concat" (fun () => {
    Iterable.concat [
        IntRange.create start::0 count::2 |> IntRange.toIterable,
        IntRange.create start::2 count::2 |> IntRange.toIterable,
        IntRange.create start::4 count::2 |> IntRange.toIterable,
      ]
      |> Iterable.take 5
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 3, 2, 1, 0];
  }),
  it "defer" (fun () => ()),
  it "distinctUntilChangedWith" (fun () => {
    [ 1, 1, 1, 2, 2, 2, 3, 3, 4, 4 ]
      |> List.toIterable
      |> Iterable.distinctUntilChangedWith Equality.int
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [ 4, 3, 2, 1 ];
  }),
  it "doOnNext" (fun () => {
    let last = ref 0;
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.doOnNext (fun i => { last := i })
      |> Iterable.forEach ignore;
    !last |> Expect.toBeEqualToInt 4;

    let empty = Iterable.empty ();
    Pervasives.(===) (Iterable.doOnNext (fun _ => ()) empty) empty
      |> Expect.toBeEqualToTrue;
  }),
  it "filter" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.filter (fun i => i mod 2 === 0)
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 2, 0];

    let empty = Iterable.empty ();
    Pervasives.(===) (Iterable.filter (fun _ => true) empty) empty
      |> Expect.toBeEqualToTrue;
  }),
  it "flatMap" (fun () => {
    IntRange.create start::0 count::3
      |> IntRange.toIterable
      |> Iterable.flatMap (fun _ => List.toIterable [1, 1, 1] )
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [1, 1, 1, 1, 1, 1, 1, 1, 1];
  }),
  it "flatten" (fun () => {
    [
      IntRange.create start::0 count::2 |> IntRange.toIterable,
      IntRange.create start::2 count::2 |> IntRange.toIterable,
      IntRange.create start::4 count::2 |> IntRange.toIterable,
    ] |> List.toIterable
      |> Iterable.flatten
      |> Iterable.take 5
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 3, 2, 1, 0];
  }),
  it "generate" (fun () => {
    Iterable.generate (fun i => i + 1) 0
      |> Iterable.skip 3
      |> Iterable.take 2
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 3];
  }),
  it "map" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.map (fun i => i * 3)
      |> Iterable.take 3
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [6, 3, 0];
  }),
  it "repeat" (fun () => {
    Iterable.generate Functions.identity 5
      |> Iterable.take 10
      |> Iterable.reduce (fun acc _ => acc + 5) 0
      |> Expect.toBeEqualToInt 50;
  }),
  it "return" (fun () => {
    Iterable.return 1
      |> Iterable.reduce (fun acc i => acc + i) 0
      |> Expect.toBeEqualToInt 1;

    Iterable.return 1
      |> Iterable.reduce while_::(fun _ i => i < 0) (fun acc i => acc + i) 0
      |> Expect.toBeEqualToInt 0;
  }),
  it "scan" (fun () => {
    []
      |> List.toIterable
      |> Iterable.scan (fun _ i => i) 0
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [0];

    [1]
      |> List.toIterable
      |> Iterable.scan (fun _ i => i) 0
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [1, 0];

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.scan (fun acc i => acc + i) 0
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [10, 6, 3, 1, 0, 0];
  }),
  it "skip" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.skip 3
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 3];

    let empty = Iterable.empty ();
    Pervasives.(===) (Iterable.skip 5 empty) empty
      |> Expect.toBeEqualToTrue;

    let x = Iterable.return 8;
    Pervasives.(===) (Iterable.skip 0 x) x
      |> Expect.toBeEqualToTrue;
  }),
  it "skipWhile" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.skipWhile (fun i => i < 3)
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 3];

    let empty = Iterable.empty ();
    Pervasives.(===) (Iterable.skipWhile (fun _ => true) empty) empty
      |> Expect.toBeEqualToTrue;
  }),
  it "startWith" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.startWith (-1)
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [4, 3, 2, 1, 0, (-1)];
  }),
  it "take" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.take 3
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [2, 1, 0];

    let empty = Iterable.empty ();
    Pervasives.(===) (Iterable.take 5 empty) empty
      |> Expect.toBeEqualToTrue;
  }),
  it "takeWhile" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.takeWhile (fun i => i < 3)
      |> Iterable.take 2
      |> List.fromReverse
      |> Expect.toBeEqualToListOfInt [1, 0];

    let empty = Iterable.empty ();
    Pervasives.(===) (Iterable.takeWhile (fun _ => true) empty) empty
      |> Expect.toBeEqualToTrue;
  }),
  it "every" (fun () => {
    Iterable.empty ()
      |> Iterable.every (fun _ => false) |> Expect.toBeEqualToTrue;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.every (fun i => i >= 0)
      |> Expect.toBeEqualToTrue;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.every (fun i => i < 3)
      |> Expect.toBeEqualToFalse;
  }),
  it "find" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.find (fun i => i ===2)
      |> Expect.toBeEqualToSomeOfInt 2;

    IntRange.empty()
      |> IntRange.toIterable
      |> Iterable.find (fun i => i ===2)
      |> Expect.toBeEqualToNoneOfInt;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.find (fun i => i ===5)
      |> Expect.toBeEqualToNoneOfInt;
  }),
  it "findOrRaise" (fun () => {
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.findOrRaise (fun i => i ===2)
      |> Expect.toBeEqualToInt 2;

    (fun () => Iterable.empty () |> Iterable.findOrRaise (fun i => i ===2))
      |> Expect.shouldRaise;
  }),
  it "forEach" (fun () => {
    let last = ref 0;
    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.forEach while_::(fun i => i < 3) (fun i => { last := i });
    !last |> Expect.toBeEqualToInt 2;
  }),
  it "none" (fun () => {
    Iterable.empty () |> Iterable.none (fun _ => false) |> Expect.toBeEqualToTrue;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.none (fun i => i >= 2)
      |> Expect.toBeEqualToFalse;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.none (fun i => i < 0)
      |> Expect.toBeEqualToTrue;
  }),
  it "some" (fun () => {
    Iterable.empty () |> Iterable.some (fun _ => false) |> Expect.toBeEqualToFalse;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.some (fun i => i >= 2)
      |> Expect.toBeEqualToTrue;

    IntRange.create start::0 count::5
      |> IntRange.toIterable
      |> Iterable.some (fun i => i < 0)
      |> Expect.toBeEqualToFalse;
  }),
];
