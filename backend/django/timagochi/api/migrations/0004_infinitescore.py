# Generated by Django 3.0.5 on 2020-05-07 02:54

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0003_auto_20200430_2042'),
    ]

    operations = [
        migrations.CreateModel(
            name='InfiniteScore',
            fields=[
                ('person', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, primary_key=True, related_name='player', serialize=False, to='api.Player')),
                ('score', models.IntegerField(default=0)),
            ],
        ),
    ]