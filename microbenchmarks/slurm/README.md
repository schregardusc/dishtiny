to submit a `JOB_NAME` on the hpcc,

```bash
sbatch <(curl -s https://raw.githubusercontent.com/mmore500/dishtiny/master/microbenchmarks/slurm/JOB_NAME+ext=.slurm.sh\?$(date +%s))
```
