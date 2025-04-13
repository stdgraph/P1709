template <forward_iterator InnerIter>
class descriptor {
public:
  using inner_iterator   = InnerIter;
  using inner_value_type = iter_value_t<inner_iterator>;
  // preserve inner value constness based on inner\_iterator
  using inner_reference = 
            conditional_t<is_const_v<remove_reference_t<decltype(*declval<inner_iterator>())>>,
                          add_lvalue_reference_t<std::add_const_t<inner_value_type>>,
                          add_lvalue_reference_t<inner_value_type>>;
  using inner_pointer = 
            conditional_t<is_const_v<remove_reference_t<decltype(*declval<inner_iterator>())>>,
                          add_pointer_t<std::add_const_t<inner_value_type>>,
                          add_pointer_t<inner_value_type>>;

  // Determine if this an index-based or iterator-based descriptor
  using id_type    = ptrdiff_t; 
  using value_type = conditional_t<random_access_iterator<inner_iterator>, id_type, inner_iterator>;

  // Honor the const/non-const contract for the value type
  using pointer       = std::add_pointer_t<value_type>;
  using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;

  using reference       = std::add_lvalue_reference_t<value_type>;
  using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;

  using difference_type = std::iter_difference_t<inner_iterator>;

  constexpr descriptor()                  = default;
  constexpr descriptor(const descriptor&) = default;
  constexpr descriptor(descriptor&&)      = default;
  constexpr ~descriptor() noexcept        = default;

  constexpr descriptor(InnerIter first, InnerIter it) : begin_(first);
  constexpr descriptor(InnerIter first, ptrdiff_t id);

  // for testing
  template <forward_range R>
  requires is_convertible_v<iterator_t<R>, InnerIter>
  constexpr descriptor(R& r, inner_iterator it).

  template <forward_range R>
  requires is_convertible_v<iterator_t<R>, InnerIter>
  constexpr descriptor(R& r, std::ptrdiff_t id = 0);

  constexpr descriptor& operator=(const descriptor&) = default;
  constexpr descriptor& operator=(descriptor&&) = default;

  // Properies
public:
  constexpr value_type& value() const noexcept;

  constexpr inner_iterator get_inner_iterator() const;

  [[nodiscard]] constexpr inner_reference inner_value() noexcept;
  [[nodiscard]] constexpr inner_reference inner_value() const noexcept;

  // Operators
public:
  //
  // dereference
  //
  // Note: range concept requirement: decltype(*descriptor) == value\_type\&
  [[nodiscard]] constexpr reference       operator*() noexcept;
  [[nodiscard]] constexpr const_reference operator*() const noexcept;

  [[nodiscard]] constexpr pointer       operator->() noexcept;
  [[nodiscard]] constexpr const_pointer operator->() const noexcept;

  //
  // operator ++ += +
  //
  constexpr descriptor& operator++();
  constexpr descriptor  operator++(int);

  constexpr descriptor& operator+=(iter_difference_t<inner_iterator> n)
  requires random_access_iterator<inner_iterator>;
  constexpr descriptor operator+(iter_difference_t<inner_iterator> n) const
  requires random_access_iterator<inner_iterator>;

  //
  // operator -- -= -
  //
  constexpr descriptor& operator--()
  requires bidirectional_iterator<inner_iterator>;
  constexpr descriptor operator--(int)
  requires bidirectional_iterator<inner_iterator>;

  constexpr descriptor& operator-=(iter_difference_t<inner_iterator> n)
  requires random_access_iterator<inner_iterator>;
  constexpr descriptor operator-(iter_difference_t<inner_iterator> n) const
  requires random_access_iterator<inner_iterator>;

  template <class InnerIter2>
  constexpr iter_difference_t<inner_iterator> operator-(const descriptor<InnerIter2>& rhs) const
  requires random_access_iterator<inner_iterator>;

  //
  // operator []
  //
  constexpr inner_reference operator[](iter_difference_t<inner_iterator> n) const
  requires random_access_iterator<inner_iterator>;

  //
  // operators ==, <=>
  //
  constexpr bool operator==(const descriptor& rhs) const noexcept;

  template <forward_iterator InnerIter2>
  requires std::equality_comparable_with<InnerIter, InnerIter2>
  constexpr bool operator==(const descriptor<InnerIter2>& rhs) const noexcept;

  // for testing; useful in general?
  template <forward_iterator InnerIter2>
  requires std::equality_comparable_with<InnerIter, InnerIter2>
  constexpr bool operator==(const InnerIter2& rhs) const noexcept;

  constexpr auto operator<=>(const descriptor& rhs) const noexcept
  requires std::integral<value_type> || std::random_access_iterator<inner_iterator>;

  template <random_access_iterator InnerIter2>
  requires std::three_way_comparable_with<InnerIter, InnerIter2> &&
           (std::integral<value_type> || random_access_iterator<inner_iterator>)
  constexpr auto operator<=>(const descriptor<InnerIter2>& rhs) const noexcept;

private:
  value_type     value_ = value_type();     // index or iterator (for exposition only)
  inner_iterator begin_ = inner_iterator(); // begin of the inner range (for exposition only)
};
