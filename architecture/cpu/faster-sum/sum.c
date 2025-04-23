int sum(int *nums, int n) {
  int total = 0;
  for (int i = 0; i < n; i++)
    total += nums[i];
  return total;
}

// int sum(int *nums, int n) {
//   int t1 = 0, t2 = 0, t3 = 0, t4 = 0;
//   int t5 = 0, t6 = 0, t7 = 0, t8 = 0;
//   int t9 = 0, t10 = 0, t11 = 0, t12 = 0;
//   int t13 = 0, t14 = 0, t15 = 0, t16 = 0;

//   for (int i = 0; i < n; i += 16) {
//     t1 += nums[i];
//     t2 += nums[i + 1];
//     t3 += nums[i + 2];
//     t4 += nums[i + 3];
//     t5 += nums[i + 4];
//     t6 += nums[i + 5];
//     t7 += nums[i + 6];
//     t8 += nums[i + 7];
//     t9 += nums[i + 8];
//     t10 += nums[i + 9];
//     t11 += nums[i + 10];
//     t12 += nums[i + 11];
//     t13 += nums[i + 12];
//     t14 += nums[i + 13];
//     t15 += nums[i + 14];
//     t16 += nums[i + 15];
//   }
//   return t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + t10 + t11 + t12 + t13 + t14 + t15 + t16;
// }
