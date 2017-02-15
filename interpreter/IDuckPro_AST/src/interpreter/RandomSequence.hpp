#ifndef RANDOMSEQUENCE_HPP_f1f3af7d_0fee_4e32_b72b_a1db581b0518
#define RANDOMSEQUENCE_HPP_f1f3af7d_0fee_4e32_b72b_a1db581b0518

class RandomSequence
{
public:
    RandomSequence();

    int operator()() const;

private:
    mutable int _next;
    static const int MAX_VALUE;
};

#endif // RANDOMSEQUENCE_HPP_f1f3af7d_0fee_4e32_b72b_a1db581b0518
