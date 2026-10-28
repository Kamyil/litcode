use trie::Trie;

#[test]
fn insert_contains() {
    let mut t = Trie::new();
    assert_eq!(t.node_count(), 1);
    assert!(t.insert("apple"));
    assert!(t.contains("apple"));
    assert!(!t.contains("app"));
    assert!(t.insert("app"));
    assert!(t.insert("app"));
    assert_eq!(t.len(), 2);
    assert_eq!(t.node_count(), 6);
    assert!(!t.insert("Bad"));
    assert!(!t.insert("zzz9"));
    assert_eq!(t.node_count(), 6);
}

#[test]
fn count_prefix() {
    let mut t = Trie::new();
    for w in ["car", "card", "care", "careful", "cat", "dog"] {
        t.insert(w);
    }
    assert_eq!(t.count_prefix("car"), 4);
    assert_eq!(t.count_prefix("ca"), 5);
    assert_eq!(t.count_prefix(""), 6);
    assert_eq!(t.count_prefix("x"), 0);
}

#[test]
fn remove_prunes() {
    let mut t = Trie::new();
    t.insert("car");
    t.insert("careful");
    assert_eq!(t.node_count(), 8);
    assert!(!t.remove("ca"));
    assert!(!t.remove("caring"));
    assert!(t.remove("careful"));
    assert_eq!(t.node_count(), 4);
    assert!(t.contains("car"));
    assert!(t.remove("car"));
    assert_eq!(t.node_count(), 1);
    assert!(t.is_empty());
}

#[test]
fn remove_keeps_shared_prefix() {
    let mut t = Trie::new();
    for w in ["car", "cart", "cat"] {
        t.insert(w);
    }
    assert!(t.remove("car"));
    assert_eq!(t.node_count(), 6);
    assert!(t.contains("cart") && t.contains("cat"));
    assert_eq!(t.count_prefix("car"), 1);
}

#[test]
fn words_with_prefix_sorted() {
    let mut t = Trie::new();
    for w in ["careful", "cat", "car", "dog", "card", "ca", "care"] {
        t.insert(w);
    }
    assert_eq!(t.words_with_prefix("ca"), ["ca", "car", "card", "care", "careful", "cat"]);
    assert!(t.words_with_prefix("z").is_empty());
    assert_eq!(t.words_with_prefix("").len(), 7);
}

#[test]
fn bulk() {
    let word = |mut x: usize| {
        let mut s = String::new();
        loop {
            s.push((b'a' + (x % 26) as u8) as char);
            x /= 26;
            if x == 0 {
                break s;
            }
        }
    };
    let mut t = Trie::new();
    for i in 0..20_000 {
        t.insert(&word(i));
    }
    assert_eq!(t.len(), 20_000);
    for i in 0..20_000 {
        assert!(t.remove(&word(i)));
    }
    assert_eq!(t.node_count(), 1);
}
